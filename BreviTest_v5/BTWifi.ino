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
  String data;
  
  if (wifiClient.connected()) {
    websocketClient.getData(data);

    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
    }
//    
//    data = "Ping at " + (String) (millis() - start_time);
//    Serial.println(data);
//    websocketClient.sendData(data, WS_OPCODE_TEXT);
//    
  }
  else {
    
    Serial.println("Client disconnected.");
    while (1) {
      // Hang on disconnect.
    }
  }
  
  // wait to start next loop
  delay(100);
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

void openWebsocket() {
  String data;
  
  // Connect to the websocket server
  if (wifiClient.connect(websocketURL, 8081)) {
    Serial.println("Connected to websocket server");
    
    // Handshake with the server
    websocketClient.path = "/brevitest";
    websocketClient.host = websocketURL;
    
    if (websocketClient.handshake(&wifiClient)) {
      Serial.println("Handshake successful");
      websocketClient.sendData("Brevitest Device v0.1", WS_OPCODE_TEXT);
    } else {
      Serial.println("Handshake failed.");
      wifiClient.stop();
    }
  }
  else {
    Serial.println("Connection failed.");
  }
}

