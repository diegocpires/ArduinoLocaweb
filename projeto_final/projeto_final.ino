#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <WebSocketsClient.h>
#include <stdio.h>
#include <ArduinoJson.h>

#define RED       5
#define GREEN    6
#define BLUE     7
long rgb[3];
long bright[3] = { 107, 67, 256};
long k;

String strJson;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 43);
WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    StaticJsonBuffer<200> jsonBuffer;
    strJson = String((char *)payload);
    JsonObject& root = jsonBuffer.parseObject(strJson);
    const char* command = root["command"];
    const char* typeMens = root["type"];

    

      switch(type) {
          case WStype_DISCONNECTED:
              Serial.println("[WSc] Disconnected!\n");
              Serial.println(Ethernet.localIP());
              delay(5000);
              webSocket.begin("192.168.1.33", 8081);
              break;
          case WStype_CONNECTED:
              {
                  Serial.print("[WSc] Connected to url: ");
                  Serial.println((char *)payload);
              }
              break;
          case WStype_TEXT:
              if(String("arduino").equals(typeMens)) {
                if(String("verde").equals(command)) {
                  analogWrite(RED,0.01);
                  analogWrite(GREEN,255* bright[2]/256);
                  analogWrite(BLUE,0.01);
                } else if(String("azul").equals(command)) {
                  analogWrite(RED,0.01);
                  analogWrite(GREEN,0.01);
                  analogWrite(BLUE,255* bright[2]/256);
                } else if(String("vermelho").equals(command)) {
                  analogWrite(RED,255* bright[2]/256);
                  analogWrite(GREEN,0.01);
                  analogWrite(BLUE,0.01);
                } else if(String("amarelo").equals(command)) {
                  analogWrite(RED,255* bright[2]/256);
                  analogWrite(GREEN,120 * bright[2]/256);
                  analogWrite(BLUE,0.01);
                }
              }
  
              break;
          case WStype_BIN:
              Serial.print("[WSc] get binary length: ");
              Serial.println(length);
              break;
      
    }
      

}

void setup () {
  Serial.begin(115200);
  for (k=0; k<3; k++) {
    pinMode(RED + k, OUTPUT);
    
  }
  Serial.println("Iniciando");
  Ethernet.begin(mac, ip);
  delay(5000);
  Serial.println(Ethernet.localIP());
  webSocket.begin("192.168.1.33", 8081);
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}
