#include <ESP8266WiFi.h>

// Some connection timeout definition
#define CONNECTION_TIMEOUT 10;

// The IP of the server in the AccessPoint mode
IPAddress accessPointIP(192, 168, 0, 254);

// Function to connect to the SSID stored.
bool connectToWifi()
{
  int timeout = CONNECTION_TIMEOUT;
  
  Serial.print("Connecting to Wifi ( SSID : '");
  Serial.print(getSSID());
  Serial.println("' )");
  // Stop previous connection
  closeServer();
  closeAPServer();
  WiFi.disconnect();  
  // Switch mode
  WiFi.mode(WIFI_STA);
  // Start connection
  WiFi.begin(getSSID().c_str(), getPassword().c_str());

  // Wait for status connected
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(500);
    timeout--;
  }
  
  // Check if connection was a success
  if( timeout > 0 )
  {
    Serial.print("Connected to '");
    Serial.print(ssid);
    Serial.print("'. IP address : ");
    Serial.println(WiFi.localIP());
  } 
  else
  {
    Serial.println("Unable to connect");
    WiFi.disconnect();  
  }

  return timeout > 0;
}

// Function returning true if the wifi is currently connected
bool isConnectedToWifi() {
  return WiFi.status() == WL_CONNECTED;
}


// Fuinction that creates an AccessPoint
bool createAccessPoint()
{
  Serial.println("Switching to an AccessPoint NavibotWifi (pass navibot)");
  
  // Stop previous connection
  closeServer();
  closeAPServer();
  WiFi.disconnect();  
  // Switch mode
  WiFi.mode(WIFI_AP);
  // Configure Access Point
  WiFi.softAPConfig(accessPointIP, accessPointIP, IPAddress(255, 255, 255, 0));
  // Create Access Point of the given name and password (8 chars min)
  bool result = WiFi.softAP("NavibotWifi", "navibot-esp");

  if(result)
  {
    Serial.print("AccesPoint created. IP address : ");
    Serial.println(WiFi.softAPIP());
  }
  else
  {
    Serial.println("Fail to create access point !");
  }
  return result;
}


