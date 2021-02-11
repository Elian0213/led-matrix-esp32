#include <Arduino.h>
#include <ArduinoJson.h>
#include <LedControl.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <WiFiClientSecure.h>

#include "config.h"

const char* ssid = WiFiSSID;
const char* password = WiFiPassword;

WebSocketsServer webSocket = WebSocketsServer(80);

int DIN = MatrixDIN;
int CS = MatrixCS;
int CLK = MatrixCLK

          LedControl lc = LedControl(DIN, CLK, CS, 0);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:
      {
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);

        int typeContext = doc["type"];

        switch (typeContext) {
          case 1:
            JsonArray ledInfo = doc["data"];
            printLED(ledInfo);

          Serial.printf("[%u] Updating LED Array...\n", num);

            break;
        }
      }
      break;
  }

}

void setup()
{
  Serial.begin(115200);

  connectWifi();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  lc.shutdown(0, false);      //The MAX72XX is in power-saving mode on startup
  lc.setIntensity(0, 15);     // Set the brightness to maximum value
  lc.clearDisplay(0);         // and clear the display
}

// Set each row to of LED's using an giving array.
void printLED(JsonArray ledArray)
{
  // Index
  int i = 0;

  for (JsonVariant data : ledArray) {
    lc.setRow(0, i, data.as<int>());
    i++;
  }
}

// Connect to the wifi network using the ESP32 Module.
void connectWifi()
{
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.println();
}

void loop()
{
  webSocket.loop();
}
