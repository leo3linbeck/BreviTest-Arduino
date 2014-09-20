//#define DEBUGGING

#include "global.h"
#include "WebSocketClient.h"
#include "sha1.h"
#include "base64.h"


bool WebSocketClient::handshake(WiFiClient *client) {
	String data;
	
    wifi_client = client;

    // If there is a connected client->
    if (wifi_client->connected()) {
        // Check request and look for websocket handshake
#ifdef DEBUGGING
            Serial.println(F("Client connected"));
#endif
        if (analyzeRequest()) {
#ifdef DEBUGGING
                Serial.println(F("Websocket established"));
#endif
                return true;

        } else {
            // Might just need to break until out of wifi_client loop.
#ifdef DEBUGGING
            Serial.println(F("Invalid handshake"));
#endif
            disconnectStream();

            return false;
        }
    } else {
        return false;
    }
}

bool WebSocketClient::analyzeRequest() {
    String temp;

    int bite;
    bool foundupgrade = false;
    unsigned long intkey[2];
    String serverKey = "----------------------------";
    char keyStart[17];
    char b64Key[25];
    String key = "------------------------258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

    randomSeed(analogRead(0));

    for (int i=0; i<16; ++i) {
        keyStart[i] = (char)random(33, 126);
    }

    base64_encode(b64Key, keyStart, 16);
    
    for (int i=0; i<24; ++i) {
        key[i] = b64Key[i];
    }

#ifdef DEBUGGING
    Serial.println(F("Sending websocket upgrade headers"));
#endif    

    wifi_client->print(F("GET "));
    wifi_client->print(path);
    wifi_client->print(F(" HTTP/1.1\r\n"));
    wifi_client->print(F("Upgrade: websocket\r\n"));
    wifi_client->print(F("Connection: Upgrade\r\n"));
    wifi_client->print(F("Host: "));
    wifi_client->print(host);
    wifi_client->print(CRLF); 
    wifi_client->print(F("Sec-WebSocket-Key: "));
    wifi_client->print(b64Key);
    wifi_client->print(CRLF);
//     wifi_client->print(F("Sec-WebSocket-Protocol: "));
//     wifi_client->print(protocol);
//     wifi_client->print(CRLF);
    wifi_client->print(F("Sec-WebSocket-Version: 13\r\n"));
    wifi_client->print(CRLF);

#ifdef DEBUGGING
    Serial.println(F("Analyzing response headers"));
#endif    

    while (wifi_client->connected() && !wifi_client->available()) {
        delay(300);
//        Serial.println("Waiting...");
    }

    // TODO: More robust string extraction
    
    while ((bite = wifi_client->read()) != -1) {

        temp += (char)bite;

        if ((char)bite == '\n') {
#ifdef DEBUGGING
            Serial.print(F("Got Header: "));
            Serial.print(temp);
#endif
            if (!foundupgrade && temp.startsWith("Upgrade: websocket")) {
                foundupgrade = true;
            }
            else {
            	if (temp.startsWith("Sec-WebSocket-Accept: ")) {
                	serverKey = temp.substring(22, temp.length() - 2); // Don't save last CR+LF
#ifdef DEBUGGING
            Serial.print(F("Server key: "));
            Serial.println(serverKey);
#endif
            	}
            }
            temp = "";		
        }

        if (!wifi_client->available()) {
          delay(20);
        }
    }

#ifdef DEBUGGING
            Serial.print(F("Key: "));
            Serial.println(key);
#endif
    uint8_t *hash;
    char result[21];
    char b64Result[30];

    Sha1.init();
    Sha1.print(key);
    hash = Sha1.result();

    for (int i=0; i<20; ++i) {
        result[i] = (char)hash[i];
    }
    result[20] = '\0';

    base64_encode(b64Result, result, 20);

#ifdef DEBUGGING
            Serial.print(F("B64 Result: "));
            Serial.println(b64Result);
#endif
    // if the keys match, good to go
    return serverKey.equals(String(b64Result));
}


bool WebSocketClient::handleStream(String& data, uint8_t *opcode) {
    uint8_t msgtype;
    uint8_t bite;
    unsigned int length;
    uint8_t mask[4];
    uint8_t index;
    unsigned int i;
    bool hasMask = false;

    if (!wifi_client->connected() || !wifi_client->available())
    {
        return false;
    }      

    msgtype = timedRead();
    if (!wifi_client->connected()) {
        return false;
    }

    length = timedRead();

    if (length & WS_MASK) {
        hasMask = true;
        length = length & ~WS_MASK;
    }


    if (!wifi_client->connected()) {
        return false;
    }

    index = 6;

    if (length == WS_SIZE16) {
        length = timedRead() << 8;
        if (!wifi_client->connected()) {
            return false;
        }
            
        length |= timedRead();
        if (!wifi_client->connected()) {
            return false;
        }   

    } else if (length == WS_SIZE64) {
#ifdef DEBUGGING
        Serial.println(F("No support for over 16 bit sized messages"));
#endif
        return false;
    }

    if (hasMask) {
        // get the mask
        mask[0] = timedRead();
        if (!wifi_client->connected()) {
            return false;
        }

        mask[1] = timedRead();
        if (!wifi_client->connected()) {

            return false;
        }

        mask[2] = timedRead();
        if (!wifi_client->connected()) {
            return false;
        }

        mask[3] = timedRead();
        if (!wifi_client->connected()) {
            return false;
        }
    }
        
    data = "";
        
    if (opcode != NULL)
    {
      *opcode = msgtype & ~WS_FIN;
    }
                
    if (hasMask) {
        for (i=0; i<length; ++i) {
            data += (char) (timedRead() ^ mask[i % 4]);
            if (!wifi_client->connected()) {
                return false;
            }
        }
    } else {
        for (i=0; i<length; ++i) {
            data += (char) timedRead();
            if (!wifi_client->connected()) {
                return false;
            }
        }            
    }
    
    return true;
}

void WebSocketClient::disconnectStream() {
#ifdef DEBUGGING
    Serial.println(F("Terminating socket"));
#endif
    // Should send 0x8700 to server to tell it I'm quitting here.
    wifi_client->write((uint8_t) 0x87);
    wifi_client->write((uint8_t) 0x00);
    
    wifi_client->flush();
    delay(10);
    wifi_client->stop();
}

bool WebSocketClient::getData(String& data, uint8_t *opcode) {
    return handleStream(data, opcode);
}    

void WebSocketClient::sendData(const char *str, uint8_t opcode) {
#ifdef DEBUGGING
    Serial.print(F("Sending data: "));
    Serial.println(str);
#endif
    if (wifi_client->connected()) {
        sendEncodedData(str, opcode);       
    }
}

void WebSocketClient::sendData(String str, uint8_t opcode) {
#ifdef DEBUGGING
    Serial.print(F("Sending data: "));
    Serial.println(str);
#endif
    if (wifi_client->connected()) {
        sendEncodedData(str, opcode);
    }
}

int WebSocketClient::timedRead() {
  while (!wifi_client->available()) {
    delay(20);  
  }

  return wifi_client->read();
}

void WebSocketClient::sendEncodedData(char *str, uint8_t opcode) {
    uint8_t mask[4];
    int size = strlen(str);
    // Opcode; final fragment
    wifi_client->write(opcode | WS_FIN);

    // NOTE: no support for > 16-bit sized messages
    if (size > 125) {
        wifi_client->write(WS_SIZE16 | WS_MASK);
        wifi_client->write((uint8_t) (size >> 8));
        wifi_client->write((uint8_t) (size & 0xFF));
    } else {
        wifi_client->write((uint8_t) size | WS_MASK);
    }

    mask[0] = random(0, 256);
    mask[1] = random(0, 256);
    mask[2] = random(0, 256);
    mask[3] = random(0, 256);
    
    wifi_client->write(mask[0]);
    wifi_client->write(mask[1]);
    wifi_client->write(mask[2]);
    wifi_client->write(mask[3]);
     
    for (int i=0; i<size; ++i) {
        wifi_client->write(str[i] ^ mask[i % 4]);
    }
}

void WebSocketClient::sendEncodedData(String str, uint8_t opcode) {
    int size = str.length() + 1;
    char cstr[size];

    str.toCharArray(cstr, size);

    sendEncodedData(cstr, opcode);
}
