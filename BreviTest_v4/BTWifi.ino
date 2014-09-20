void wifi_setup() {
  Serial.println(F("Setting up wifi connection..."));
  boolean wifiConnected = connectToWiFiNetwork();
  if (wifiConnected) {
    openWebsocket();
  }
  else {
    Serial.println(F("Wifi connection failed. Server not started."));
    while (true);
  }
}

void wifi_loop() {

  unsigned long lasthello;

  websocketClient.monitor();

  unsigned long now = millis();
  if ((now - lasthello) >= HELLO_INTERVAL) {
    lasthello = now;
    if (websocketClient.connected()) {
      websocketClient.send("Hello, world!\n");
    }
  }
}

boolean connectToWiFiNetwork() {
  // attempt to connect using WPA2 encryption:
  Serial.println(F("Attempting to connect to WPA network..."));
  wifi_status = WiFi.begin(ssid, pass);

  // if you're not connected, stop here:
  if (wifi_status != WL_CONNECTED) { 
      Serial.println(F("Couldn't connect, trying alternate network..."));
    wifi_status = WiFi.begin(ssid2, pass2);
    if (wifi_status != WL_CONNECTED) { 
      Serial.println(F("Couldn't get a wifi connection"));
      return false;
    }
    else {
      Serial.print(F("Connected to network "));
      Serial.println(ssid2);
    }
  }
  else {
    Serial.print(F("Connected to network "));
    Serial.println(ssid);
  }
  ip_address = WiFi.localIP();
  Serial.print(F("IP address: "));
  Serial.println(ip_address);
  return true;
}

// Here we define a maximum framelength to 64 bytes. Default is 256.
#define MAX_FRAME_LENGTH 64

// Define how many callback functions you have. Default is 1.
#define CALLBACK_FUNCTIONS 1

void ondata(SocketIOClient client, char *data) {
  Serial.print(data);
}

void openWebsocket() {
// websocket message handler: do something with command from server

  websocketClient.setDataArrivedDelegate(ondata);
  if (!websocketClient.connect("ws://127.0.0.1/brevitest", 8081)) {
    Serial.println(F("Not connected."));
  }

  if (websocketClient.connected()) {
    websocketClient.send("Client here!");
  }
}

