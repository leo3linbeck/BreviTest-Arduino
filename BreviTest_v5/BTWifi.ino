void wifi_setup() {
  boolean wifiConnected = connectToWiFiNetwork();
  if (wifiConnected) {
    openWebsocket();
  }
  else {
    Serial.println(F("Wifi connection failed."));
    while (true);
  }
}

void add_to_queue(String data) {
  if (message_queue_length < MAX_QUEUE_SIZE) {
    message_queue[message_queue_length] = data;
    message_queue_length += 1;
  }
  else {
    Serial.println(F("Message queue full.")); 
  }
}

void wifi_loop() {
  String data;
  
  if (wifiClient.connected()) {
    
    websocketClient.getData(data);
    while (data.length() > 0) {
      add_to_queue(data);
      Serial.print(F("Received data: "));
      Serial.println(data);
      data = "";
      websocketClient.getData(data);
    }
    
    pingWebsocket();  
  }
  else {
    Serial.println(F("Client disconnected."));
    while (1) {
      // Hang on disconnect.
    }
  }
}

void pingWebsocket() {
  unsigned long elapsed_time = millis() - start_time;
  if ((elapsed_time - last_ping) > PING_INTERVAL) {
    last_ping = elapsed_time;
    websocketClient.sendData("PING");
  }
}

boolean connectToWiFiNetwork() {
  // attempt to connect using WPA2 encryption:
  wifi_status = WiFi.begin(ssid, pass);

  // if you're not connected, stop here:
  if (wifi_status != WL_CONNECTED) { 
    return false;
  }
  else {
    Serial.print(F("Network name: "));
    Serial.println(ssid);
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
    return true;
  }
}

void openWebsocket() {
  String data;
  
  // Connect to the websocket server
  if (wifiClient.connect(websocketURL, websocketPort)) {
    Serial.println(F("Connected to websocket server"));
    
    // Handshake with the server
    websocketClient.path = websocketPath;
    websocketClient.host = websocketURL;
    
    if (websocketClient.handshake(&wifiClient)) {
      Serial.println(F("Handshake successful"));
      delay(500);
      websocketClient.sendData(String("INIT:DEVICE:") + String(serial_number));
    }
    else {
      Serial.println(F("Handshake failed."));
      wifiClient.stop();
    }
  }
  else {
    Serial.println(F("Connection failed."));
  }
}

