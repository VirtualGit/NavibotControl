#include <ESP8266WebServer.h>

// Create a server on port 80
ESP8266WebServer serverAP(80);

void createAPServer()
{
  // Catch root request. Return a form fo the user to fill in
  // the SSID to connect to, and its password.
  serverAP.on("/", [](){
    serverAP.send(200, "text/html", "<html>\
<body>\
<h1>Navibot Wifi configuration</h1>\
<form action='/setSSID' method='post'>\
<p>SSID : <input type='text' name='ssid'/></p>\
<p>PASSWORD : <input type='password' name='password'/></p>\
<p><input type='submit' value='Confirm'/></p>\
</form>\
</body>\
</html>");
  });

  // Catch the request of form sent
  serverAP.on("/setSSID", [](){
    serverAP.send(200, "text/html", "<html>\
<body>\
<h1>Ok</h1>\
</body>\
</html>");

    // Read arguments from the filled form
    bool ssidOk = false;
    bool passOk = false;
    for (int i = 0; i < serverAP.args(); i++) {
      // Intercept ssid
      if( serverAP.argName(i) == "ssid" )
      {
        ssidOk = true;
        setSSID( serverAP.arg(i) );
      }
      // Intercept password
      if( serverAP.argName(i) == "password" )
      {
        passOk = true;
        setPassword( serverAP.arg(i) );
      }
    } 

    if( ssidOk && passOk )
    {
      // We need both SSID and PASS to be given by the form.
      // Log the reconnection
      Serial.println("Will reconnect to "+getSSID());
      // Store the new SSID and password
      writeSettings();
      // Close the current server
      serverAP.close();
      // And set the mode to the RECONNECT state (will then connect to the changed SSID)
      setMode( RECONNECT );
    }
    else
    {
      // Not complete request (should not happen)
      Serial.println(String("Request not complete ( SSID : ")+(ssidOk?"OK":"Missing")+" Password : "+(passOk?"OK":"Missing"));
    }
  });

  // Start the server and listen to requests
  serverAP.begin();
}

// Close the server
void closeAPServer()
{
  serverAP.close();
}

// Handle clients
void ServerAPHandleClient()
{ 
  serverAP.handleClient();
}

