
// Delay to retry a wifi connection in cas it has previously failed
#define DELAI_TENTATIVE 300

// Global mode enum
typedef enum {
  NONE,
  NORMAL,
  ACCESSPOINT,
  ERROR,
  RECONNECT
} EMode;

// Global variable of the global mode
EMode currentMode = NONE;
// The timestamp when the current mode has been set
unsigned long timeMode = millis();

// Function to change the current mode
void setMode( EMode mode )
{
  currentMode = mode;
  timeMode = millis();
}

// Function to get the time (in seconds) that the
// current mode has been set
unsigned long getModeTimeSeconds()
{
  return (millis() - timeMode) / 1000;
}


// Main setup function (initialization)
void setup(void){
  // Initialize the Serial port
  Serial.begin(74880);
  // Wait for a moment 
  unsigned long start = millis();
  while (!Serial && (millis()-start)<3000);

  // Log the starting process
  Serial.println("========================");
  Serial.println("Starting Navibot ESP");
  Serial.println("========================");
  delay(2000);

  // Initialize settings module
  SettingsBegin();
  // Initialize IR module
  IRBegin();
  // Start the main connection strategy
  mainConnectStrategy();
}

// This function handle the main connection strategy.
// It will try to connect to the stored Wifi SSID.
// If it fail, it will start an AccesPoint server for
// the user can login and set the SSID to connect to.
void mainConnectStrategy()
{
  Serial.println("Starting the connection strategy :");
  if( !connectToWifi() )
  {
    // In case of failure, create an accesspoint to
    // change settings
    setMode( ACCESSPOINT );
    if( createAccessPoint() )
    {
      // Create the server to change the settings
      createAPServer();
    }
    else
    {
      // Cannot connect neither create access point.
      Serial.println("Execution won't go further !");
      setMode( ERROR );
    }
    timeMode = millis();
  }
  else
  {
    // Connected to host wifi, create the normal server
    setMode( NORMAL );
    createServer();
  }
}


// Main loop function
void loop(void){
  
  switch( currentMode )
  {
    case NORMAL:
      // Normal mode, a lost connection may restart the connection strategy
      if( !isConnectedToWifi() && getModeTimeSeconds() > DELAI_TENTATIVE )
      {
        mainConnectStrategy();
      }
      // Handle clients
      ServerHandleClient();
      break;
    case ACCESSPOINT:
      // After a timeout, we may restart the connection strategy
      if( getModeTimeSeconds() > DELAI_TENTATIVE )
      {
        mainConnectStrategy();
      }
      ServerAPHandleClient();
      break;
    case RECONNECT:
      mainConnectStrategy();
      break;
    case NONE:
    default:
      // We can reach this point if neither SSID nor AP succed. 
      // Re-try to connect to the known wifi
      if( getModeTimeSeconds() > DELAI_TENTATIVE )
      {
        mainConnectStrategy();
      }
  
  }

  delay(500);
}




