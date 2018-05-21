#include <EEPROM.h>

// The SSID to connect to and its PASSWORD
String ssid = "";
String password = "";

// Initialization of this module
void SettingsBegin()
{  
  EEPROM.begin(256);
  //RAZSettings();
  readSettings();
}

// Read the settings from EEPROM to RAM
void readSettings()
{
  char c;
  ssid = "";
  password = "";
  for(int i = 0; i < 128 ; i++ )
  {
    c = char(EEPROM.read(i));
    if( !c ) break;
    ssid += c;
  }
  for(int i = 0; i < 128 ; i++ )
  {
    c = char(EEPROM.read(128+i));
    if( !c ) break;
    password += c;
  }
  Serial.println("Reading settings");
  Serial.print("SSID read : ");
  Serial.println(ssid);
}

// Write the settings from RAM to EEPROM
void writeSettings()
{
  for (int i = 0; i < 256; ++i) { EEPROM.write(i, 0); }
  for (int i = 0; i < ssid.length(); ++i)
  {
    EEPROM.write(i, ssid[i]);
  }
  for (int i = 0; i < password.length(); ++i)
  {
    EEPROM.write(128+i, password[i]);
  } 
  EEPROM.commit();
}


void RAZSettings()
{
  for (int i = 0; i < 256; ++i) { EEPROM.write(i, 0); }
  EEPROM.commit();
}

String& getSSID() { return ssid; }
String& getPassword() { return password; }

void setSSID( String newSSID ) { ssid = newSSID; }
void setPassword( String newPassword ) { password = newPassword; }
