#ifdef GSM_ON
  bool gsm_setup() {
     return (connectToGSMNetwork() && openWebsocket());
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
  
  void gsm_loop() {
    String data;
    
    if (gsmClient.connected()) {
      
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
  
  bool connectToGSMNetwork() {
    boolean notConnected = true;
    
    while(notConnected)
    {
      Serial.println("Starting GSM...");
      if (gsmAccess.begin(PINNUMBER)==GSM_READY) {
         Serial.println("Attaching GPFS...");
         if (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD)==GPRS_READY) {
           notConnected = false;
           Serial.println("Connected to GSM/GPRS network");
         }
      }
      else
      {
        Serial.println("Not connected");
        delay(1000);
      }
    }
  }
  
  bool openWebsocket() {
    // Connect to the websocket server
    if (gsmClient.connect(websocketURL, websocketPort)) {
      Serial.println(F("Connected to websocket server"));
      
      // Handshake with the server
      websocketClient.path = websocketPath;
      websocketClient.host = websocketURL;
      
      if (websocketClient.handshake(&gsmClient)) {
        return true;
     }
      else {
        Serial.println(F("Handshake failed."));
        gsmClient.stop();
     }
    }
    else {
      Serial.println(F("Connection failed."));
    }
    
    return false;
  }
#endif

