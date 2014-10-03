//#define DEBUGGING

#include "global.h"
#include "WebSocketClient.h"
#include "sha1.h"
#include "base64.h"


bool WebSocketClient::handshake(GSMClient *client) {
	String data;
	
    gsm_client = client;

    // If there is a connected client->
    if (gsm_client->connected()) {
        // Check request and look for websocket handshake
#ifdef DEBUGGING
            Serial.println(F("Client connected"));
#endif
        if (analyzeRequest()) {
#ifdef DEBUGGING
                Serial.println(F("Websocket established"));
#endif
                return true;

        }
        else {
            // Might just need to break until out of gsm_client loop.
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

    gsm_client->print(F("GET "));
    gsm_client->print(path);
    gsm_client->print(F(" HTTP/1.1\r\n"));
    gsm_client->print(F("Upgrade: websocket\r\n"));
    gsm_client->print(F("Connection: Upgrade\r\n"));
    gsm_client->print(F("Origin: Arduino\r\n"));
    gsm_client->print(F("Host: "));
    gsm_client->print(host);
    gsm_client->print(CRLF); 
    gsm_client->print(F("Sec-WebSocket-Key: "));
    gsm_client->print(b64Key);
    gsm_client->print(CRLF);
//    gsm_client->print(F("Sec-WebSocket-Protocol: chat\r\n"));
    gsm_client->print(F("Sec-WebSocket-Version: 13\r\n"));
    gsm_client->print(CRLF);

#ifdef DEBUGGING
    Serial.println(F("Analyzing response headers"));
#endif    

    while (gsm_client->connected() && !gsm_client->available()) {
        delay(300);
//        Serial.println("Waiting...");
    }

    // TODO: More robust string extraction
    
    while ((bite = gsm_client->read()) != -1) {

        temp += (char)bite;

        if ((char)bite == '\n') {
#ifdef DEBUGGING
            Serial.print(F("Got Header: "));
            Serial.print(temp);
#endif
			if (temp.startsWith("Sec-WebSocket-Accept")) {
				serverKey = temp.substring(22, temp.length() - 2); // Don't save last CR+LF
#ifdef DEBUGGING
				Serial.print(F("Server key: "));
				Serial.println(serverKey);
#endif
			}
            else if (!foundupgrade && temp.startsWith("Upgrade: websocket")) {
                foundupgrade = true;
            }
            temp = "";		
        }

        if (!gsm_client->available()) {
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

    if (!gsm_client->connected() || !gsm_client->available())
    {
        return false;
    }      

    msgtype = timedRead();
    if (!gsm_client->connected()) {
        return false;
    }

    length = timedRead();

    if (length & WS_MASK) {
        hasMask = true;
        length = length & ~WS_MASK;
    }


    if (!gsm_client->connected()) {
        return false;
    }

    index = 6;

    if (length == WS_SIZE16) {
        length = timedRead() << 8;
        if (!gsm_client->connected()) {
            return false;
        }
            
        length |= timedRead();
        if (!gsm_client->connected()) {
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
        if (!gsm_client->connected()) {
            return false;
        }

        mask[1] = timedRead();
        if (!gsm_client->connected()) {

            return false;
        }

        mask[2] = timedRead();
        if (!gsm_client->connected()) {
            return false;
        }

        mask[3] = timedRead();
        if (!gsm_client->connected()) {
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
            if (!gsm_client->connected()) {
                return false;
            }
        }
    } else {
        for (i=0; i<length; ++i) {
            data += (char) timedRead();
            if (!gsm_client->connected()) {
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
    gsm_client->write((uint8_t) 0x87);
    gsm_client->write((uint8_t) 0x00);
    
    gsm_client->flush();
    delay(10);
    gsm_client->stop();
}

bool WebSocketClient::getData(String& data, uint8_t *opcode) {
    return handleStream(data, opcode);
}    

void WebSocketClient::sendData(const char *str, uint8_t opcode) {
#ifdef DEBUGGING
    Serial.print(F("Sending data: "));
    Serial.println(str);
#endif
    if (gsm_client->connected()) {
        sendEncodedData(str, opcode);       
    }
}

void WebSocketClient::sendData(String str, uint8_t opcode) {
#ifdef DEBUGGING
    Serial.print(F("Sending data: "));
    Serial.println(str);
#endif
    if (gsm_client->connected()) {
        sendEncodedData(str, opcode);
    }
}

int WebSocketClient::timedRead() {
  while (!gsm_client->available()) {
    delay(20);  
  }

  return gsm_client->read();
}

void WebSocketClient::sendEncodedData(char *str, uint8_t opcode) {
    uint8_t mask[4];
    int size = strlen(str);

    if (size > 125) {
    	Serial.println(F("String too long for sending through websocket"));
    }
    else {
		// Opcode; final fragment
		gsm_client->write(opcode | WS_FIN);
		gsm_client->write(((uint8_t) size) | WS_MASK);

		mask[0] = random(0, 256);
		mask[1] = random(0, 256);
		mask[2] = random(0, 256);
		mask[3] = random(0, 256);
	
		gsm_client->write(mask[0]);
		gsm_client->write(mask[1]);
		gsm_client->write(mask[2]);
		gsm_client->write(mask[3]);
	 
		for (int i=0; i<size; ++i) {
			gsm_client->write(str[i] ^ mask[i % 4]);
		}
    }
}

void WebSocketClient::sendEncodedData(String str, uint8_t opcode) {
    int size = str.length() + 1;
    char cstr[size];
    
// 	Serial.print(F("sendEncodedData: "));
// 	Serial.println(str);
	
    str.toCharArray(cstr, size);

    sendEncodedData(cstr, opcode);
}
