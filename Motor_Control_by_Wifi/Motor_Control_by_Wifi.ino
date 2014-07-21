#include <Stepper.h>
#include <WiFi.h>

char ssid[] = "Linbeck Home";     //  your network SSID (name) 
char pass[] = "2january88";    // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
WiFiClient client;

boolean connectToWiFiNetwork() {
  // initialize serial:
  Serial.begin(115200);

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
    Serial.print("Connected to network ");
    Serial.println(ssid);
    return true;
  }
}

void setup() {
  boolean wifiConnected;
  
  wifiConnected = connectToWiFiNetwork();
  if (!wifiConnected) {
    while (true);
  }
  else {
  }
}

void loop() {
  // do nothing
}
 

