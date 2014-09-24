#ifdef WIFI_ON
  bool wifi_setup() {
     return (connectToWiFiNetwork() && openWebsocket());
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
        Serial.println(F("Websocket message received from server."));
        add_to_queue(data);
        data = "";
        websocketClient.getData(data);
      }
      
      pingWebsocket();  
    }
    else {
      Serial.println(F("Client disconnected."));
      while (true);
    }
  }
  
  void pingWebsocket() {
    unsigned long elapsed_time = millis() - start_time;
    if ((elapsed_time - last_ping) > PING_INTERVAL) {
      last_ping = elapsed_time;
      websocketClient.sendData("PING");
    }
  }
  
  bool connectToWiFiNetwork() {
    // attempt to connect using WPA2 encryption:
    wifi_status = WiFi.begin(ssid, pass);
  
    // if you're not connected, stop here:
    if (wifi_status != WL_CONNECTED) { 
      Serial.println(F("Wifi connection failed."));
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
  
  bool openWebsocket() {
    // Connect to the websocket server
    if (wifiClient.connect(websocketURL, websocketPort)) {
      Serial.println(F("Connected to websocket server"));
      
      // Handshake with the server
      websocketClient.path = websocketPath;
      websocketClient.host = websocketURL;
      
      if (websocketClient.handshake(&wifiClient)) {
        return true;
     }
      else {
        Serial.println(F("Handshake failed."));
        wifiClient.stop();
     }
    }
    else {
      Serial.println(F("Connection failed."));
    }
    
    return false;
  }
#endif

