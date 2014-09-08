void wifi_setup() {
  Serial.println(F("Setting up wifi connection..."));
  boolean wifiConnected = connectToWiFiNetwork();
  if (wifiConnected) {
    startServer();
  }
  else {
    Serial.println(F("Wifi connection failed. Server not started."));
    while (true);
  }
}

void wifi_loop() {
  // Create a client connection
  WiFiClient client = server.available();
  if (client) {
    Serial.println(F("New client connection"));
    boolean currentLineIsBlank = true;
    httpRequest="";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
 
        //store characters to string
        httpRequest += c;
 
        //if HTTP request has ended
        if (c == '\n' && currentLineIsBlank) {
           Serial.println(F("HTTP request:"));
           Serial.println(httpRequest);
 
           if (httpRequest.indexOf("run_brevitest") > 0) {
              run_brevitest();
           }
           else {
             if (httpRequest.indexOf("read_sensors") > 0) {
                get_sensor_readings();
             }
             else {  // HTTP request for web page
                // send web page - contains JavaScript with AJAX calls
                serveWebPage(client);
             }
           }
          //clearing string for next read
          httpRequest="";
          break;
        } // end of HTTP processing
       
         // every line of text received from the client ends with \r\n
        if (c == '\n') {
            // last character on line of received text
            // starting new line with next character read
            currentLineIsBlank = true;
        } 
        else if (c != '\r') {
            // a text character was received from client
            currentLineIsBlank = false;
        }
     } // end if (client.available())
    } // end while (client.connected())
    
    delay(1);
    //stopping client
    client.stop();
    Serial.println(F("Client disconnected"));
  } // end if (client)
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

void startServer() {
  Serial.println(F("Starting up server..."));
  server.begin();
  Serial.println(F("Server started"));
}

//  LOOP functions

void serveWebPage(WiFiClient client) {
  Serial.println(F("Start serveWebPage"));
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println(F("Connection: keep-alive"));
  client.println();
  client.println(F("<!DOCTYPE HTML>"));
  client.println(F("<HTML>"));
  client.println(F("<HEAD>"));
  client.println(F("<TITLE>BreviTest Console</TITLE>"));
  client.print(F("<link rel='stylesheet' "));
  client.print(F("href='http://www.csszengarden.com/examples/style.css' "));
  client.println(F("type='text/css'>"));
  client.println(F("<script>"));
  client.println(F("function callBreviTestFunction(func_name) {"));
  client.println(F("nocache = '&nocache=' + Math.random() * 1000000;"));
  client.println(F("var request = new XMLHttpRequest();"));
  client.println(F("request.open('GET', func_name + nocache, true);"));
  client.println(F("request.send(null);"));
  client.println(F("}"));
  client.println(F("</script>"));
  client.println(F("</HEAD>"));
  client.println(F("<BODY>"));
  client.println(F("<H1>BreviTest Test Console</H1>"));
  client.print(F("<H2>IP Address: "));
  client.print(WiFi.localIP());
  client.println(F("</H2>"));
  client.println(F("<hr />"));
  client.println(F("<br />"));
  client.print(F("<button type='button' "));
  client.print(F("onclick=callBreviTestFunction('run_brevitest')>"));
  client.println(F("Run BreviTest</button>"));
  client.print(F("<button type='button' "));
  client.print(F("onclick=callBreviTestFunction('read_sensors')>"));
  client.println(F("Read Sensors</button>"));
  client.println(F("</BODY>"));
  client.println(F("</HTML>"));
  Serial.println(F("End serveWebPage"));
}


