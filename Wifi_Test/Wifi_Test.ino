#include <WiFi.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins

#define pinMotorEnableA 9 // 9 on the shield
#define pinMotorEnableB 8 // 8 on the shield
#define pinMotorDirectionA 6 // 12 on the shield
#define pinMotorDirectionB 4 // 13 on the shield
#define pinMotorPowerA 3 // 3 on the shield
#define pinMotorPowerB 5 // 11 on the shield

int stepsPerRotation = 200.0;
int rpm = 30.0;
int delayLength = int (1000.0 / (stepsPerRotation * rpm / 60.0));
char ssid[] = "iAlphaDev";     //  your network SSID (name) 
char pass[] = "ialpha123";    // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
WiFiServer server(80);
String httpRequest;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // initialize serial:
  Serial.begin(9600);
 
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

  // establish PWM pins
  pinMode(pinMotorPowerA, OUTPUT); //move CH A
  pinMode(pinMotorPowerB, OUTPUT); //move CH B

  // establish motor direction toggle pins
  pinMode(pinMotorDirectionA, OUTPUT); //CH A -- HIGH = forwards and LOW = backwards???
  pinMode(pinMotorDirectionB, OUTPUT); //CH B -- HIGH = forwards and LOW = backwards???
  
  // establish motor brake pins
  pinMode(pinMotorEnableA, OUTPUT); //brake (disable) CH A
  pinMode(pinMotorEnableB, OUTPUT); //brake (disable) CH B
 
  boolean wifiConnected = connectToWiFiNetwork();
  if (wifiConnected) {
    startServer();
  }
  else {
    Serial.println("Wifi connection failed. Server not started.");
    while (true);
  }
}

void loop() {
  // Create a client connection
  WiFiClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    httpRequest="";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
 
        //store characters to string
        httpRequest += c;
 
        //if HTTP request has ended
        if (c == '\n' && currentLineIsBlank) {
           ///////////////////// control motor
//           Serial.println(httpRequest);
 
           if(httpRequest.indexOf("rotate_forward") > 0) {
              moveForwardOneRotation();
            }
            else if(httpRequest.indexOf("rotate_backward") > 0) {
              moveBackwardOneRotation();
            }
            else {  // HTTP request for web page
            // send web page - contains JavaScript with AJAX calls
            serveWebPage(client);
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
    
    delay(10);
    //stopping client
    client.stop();
  } // end if (client)
}

//  SETUP functions

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
    Serial.print("Connected to network ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  }
}

void startServer() {
  server.begin();
}

//  LOOP functions

void serveWebPage(WiFiClient client) {
//   Serial.println(readString); //print to serial monitor for debugging
 
  client.println("HTTP/1.1 200 OK"); //send new page
  client.println("Content-Type: text/html");
  client.println("Connection: keep-alive");
  client.println();
   
  client.println("<HTML>");
  client.println("<HEAD>");
  client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
  client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
  client.println("<link rel='stylesheet' type='text/css' href='http://homeautocss.net84.net/a.css' />");
  client.println("<TITLE>Rotate The Stepper</TITLE>");
  client.println("<script>");
  client.println("function AjaxRotate(dir) {");
  client.println("var request = new XMLHttpRequest();");
  client.println("request.open('GET', dir, true);");
  client.println("request.send(null);");
  client.println("}");
  client.println("</script>");
  client.println("</HEAD>");
  client.println("<BODY>");
  client.println("<H1>Rotate The Stepper</H1>");
  client.println("<hr />");
  client.println("<br />");
  client.println("<button type='button' onclick=AjaxRotate('rotate_forward')>Rotate Forward</button>");
  client.println("<button type='button' onclick=AjaxRotate('rotate_backward')>Rotate Backward</button>");

  client.println("</BODY>");
  client.println("</HTML>");
}

void moveForwardOneRotation() {
  Serial.println("Forward one rotation...");
  for (int i = 0; i < stepsPerRotation; i += 1) {
    moveForwardOneStep(i % 4);
  }
}

void moveBackwardOneRotation() {
  Serial.println("Backward one rotation...");
  for (int i = 0; i < stepsPerRotation; i += 1) {
    moveBackwardOneStep(i % 4);
  }
}

void moveForwardOneStep(int mode) {
  if (mode == 0) {
    digitalWrite(pinMotorEnableA, LOW);  //ENABLE CH A
    digitalWrite(pinMotorEnableB, HIGH); //DISABLE CH B
    digitalWrite(pinMotorDirectionA, HIGH);   //Sets direction of CH A
    digitalWrite(pinMotorPowerA, HIGH);   //Moves CH A
  }
  else if (mode == 1) {
    digitalWrite(pinMotorEnableA, HIGH);  //DISABLE CH A
    digitalWrite(pinMotorEnableB, LOW); //ENABLE CH B
    digitalWrite(pinMotorDirectionB, LOW);   //Sets direction of CH B
    digitalWrite(pinMotorPowerB, HIGH);   //Moves CH B
  }
  else if (mode == 2) {
    digitalWrite(pinMotorEnableA, LOW);  //ENABLE CH A
    digitalWrite(pinMotorEnableB, HIGH); //DISABLE CH B
    digitalWrite(pinMotorDirectionA, LOW);   //Sets direction of CH A
    digitalWrite(pinMotorPowerA, HIGH);   //Moves CH A
  }
  else if (mode == 3) {
    digitalWrite(pinMotorEnableA, HIGH);  //DISABLE CH A
    digitalWrite(pinMotorEnableB, LOW); //ENABLE CH B
    digitalWrite(pinMotorDirectionB, HIGH);   //Sets direction of CH B
    digitalWrite(pinMotorPowerB, HIGH);   //Moves CH B
  }
  delay(delayLength);
}

void moveBackwardOneStep(int mode) {
  if (mode == 0) {
    digitalWrite(pinMotorEnableA, LOW);  //ENABLE CH A
    digitalWrite(pinMotorEnableB, HIGH); //DISABLE CH B
    digitalWrite(pinMotorDirectionA, HIGH);   //Sets direction of CH A
    digitalWrite(pinMotorPowerA, HIGH);   //Moves CH A
  }
  else if (mode == 1) {
    digitalWrite(pinMotorEnableA, HIGH);  //DISABLE CH A
    digitalWrite(pinMotorEnableB, LOW); //ENABLE CH B
    digitalWrite(pinMotorDirectionB, HIGH);   //Sets direction of CH B
    digitalWrite(pinMotorPowerB, HIGH);   //Moves CH B
  }
  else if (mode == 2) {
    digitalWrite(pinMotorEnableA, LOW);  //ENABLE CH A
    digitalWrite(pinMotorEnableB, HIGH); //DISABLE CH B
    digitalWrite(pinMotorDirectionA, LOW);   //Sets direction of CH A
    digitalWrite(pinMotorPowerA, HIGH);   //Moves CH A
  }
  else if (mode == 3) {
    digitalWrite(pinMotorEnableA, HIGH);  //DISABLE CH A
    digitalWrite(pinMotorEnableB, LOW); //ENABLE CH B
    digitalWrite(pinMotorDirectionB, LOW);   //Sets direction of CH B
    digitalWrite(pinMotorPowerB, HIGH);   //Moves CH B
  }
  delay(delayLength);
}
