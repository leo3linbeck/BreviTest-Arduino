#include <WiFi.h>
#include "SPI.h"
#include <SocketIOClient.h>

#define DEBUG

char server[] = "echo.websocket.org/";
char ssid[] = "Linbeck Home";     //  your network SSID (name) 
char pass[] = "2january88";    // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
SocketIOClient client;

boolean connectToWiFiNetwork() {
  // attempt to connect using WPA2 encryption:
  Serial.println("Attempting to connect to WPA network...");
  status = WiFi.begin(ssid, pass);

  // if you're not connected, stop here:
  if ( status != WL_CONNECTED) { 
    Serial.println("Couldn't get a wifi connection");
    return false;
  } 
  // if you are connected, print out info about the connection:
  else {
    Serial.println("Connected to network");
    Serial.print("Firmware version: ");
    Serial.println(WiFi.firmwareVersion());
    return true;
  }
}

void setup() {
  Serial.begin(57600);
  connectToWiFiNetwork();
  Serial.println("Connecting to websocket...");
  if (client.connect(server, 80)) {
    Serial.println("Setting data arrival delegate");
    client.setDataArrivedDelegate(dataArrived);
    Serial.println("Sending websocket message");
    client.send("Hello World!");
  }
  else {
    Serial.println("Websocket connect failed!");
  }
}

void loop() {
  client.monitor();
}

void dataArrived(SocketIOClient client, char *data) {
  String s = "Data Arrived: ";
  
  Serial.println(s + data);
}

