#include <ESP8266WebServer.h>

// Create a server on port 80
ESP8266WebServer server(80);

void createServer()
{
  // Catch root request, but do nothing. You could add links here.
  server.on("/", [](){
    server.send(200, "text/html", "");
  });
  // Catch start request in MAXIMUM mode
  server.on("/startComplete", [](){
    server.send(200, "text/html", "Starting Complete");
    IRSendStartComplete();   
  });
  // Catch start request in EDGE mode
  server.on("/startEdge", [](){
    server.send(200, "text/html", "Starting Edge");
    IRSendStartEdge();
  });

  // Start the server and listen to requests
  server.begin();
}

// Close the server
void closeServer()
{
  server.close();
}

// Handle clients
void ServerHandleClient()
{ 
  server.handleClient();
}

