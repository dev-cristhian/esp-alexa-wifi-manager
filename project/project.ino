#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Espalexa.h>

const int PIN_CONNECTED_LED = 0;
const int PIN_LIGHT = 1;
const int PIN_RESET_WIFI = 2;
const int PIN_ERROR_LED = 3;

byte RESETED = LOW;

Espalexa ESP_ALEXA;
WiFiManager wifiManager;

void setup() {
  startConfigPins();

  wifiManager.autoConnect("ESPAutoConnected", "12345678Qq!");
  delay(1000);

  WiFi.begin();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(PIN_CONNECTED_LED, HIGH);
    digitalWrite(PIN_ERROR_LED, LOW);

    ESP_ALEXA.addDevice("ESP", deviceOne);
    ESP_ALEXA.begin();
  }
}

void loop() {
  if (!digitalRead(PIN_RESET_WIFI)) {
    RESETED = HIGH;
  }



  if (!RESETED) {
    ESP_ALEXA.loop();
    delay(1);
  } else {
    RESETED = LOW;
    wifiManager.resetSettings();
    ESP.eraseConfig();
    delay(2000);
    ESP.reset();
  }
}

void startConfigPins() {
  pinMode(PIN_CONNECTED_LED, OUTPUT);
  pinMode(PIN_LIGHT, OUTPUT);
  pinMode(PIN_ERROR_LED, OUTPUT);

  pinMode(PIN_RESET_WIFI, INPUT);

  digitalWrite(PIN_CONNECTED_LED, LOW);
  digitalWrite(PIN_LIGHT, LOW);
  digitalWrite(PIN_ERROR_LED, HIGH);
}

void deviceOne(uint8_t brightness) {
  digitalWrite(PIN_LIGHT, brightness);
}