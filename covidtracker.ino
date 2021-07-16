#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "config.h"

String url = "https://covid-api.com/api/reports?region_name=";
String country = "Malaysia";

ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, pass);

}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;
    WiFiClientSecure client;
    client.setInsecure();

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, url+country)) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          //Serial.println(payload);
          decodejsonandprint(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }

  delay(10000);
}

void decodejsonandprint(String payload)
{
  StaticJsonDocument<768> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  JsonObject data_0 = doc["data"][0];
  const char* data_0_date = data_0["date"]; // "2021-07-15"
  long data_0_confirmed = data_0["confirmed"]; // 880782
  int data_0_deaths = data_0["deaths"]; // 6613
  long data_0_recovered = data_0["recovered"]; // 765800
  int data_0_confirmed_diff = data_0["confirmed_diff"]; // 13215
  int data_0_deaths_diff = data_0["deaths_diff"]; // 110
  int data_0_recovered_diff = data_0["recovered_diff"]; // 6095
  const char* data_0_last_update = data_0["last_update"]; // "2021-07-16 04:21:22"
  long data_0_active = data_0["active"]; // 108369
  int data_0_active_diff = data_0["active_diff"]; // 7010
  float data_0_fatality_rate = data_0["fatality_rate"]; // 0.0075

  JsonObject data_0_region = data_0["region"];
  const char* data_0_region_iso = data_0_region["iso"]; // "MYS"
  const char* data_0_region_name = data_0_region["name"]; // "Malaysia"
  const char* data_0_region_province = data_0_region["province"]; // ""
  const char* data_0_region_lat = data_0_region["lat"]; // "4.2105"
  const char* data_0_region_long = data_0_region["long"]; // "101.9758"
  Serial.println("Confirmed covid cases in "+String(data_0_region_name)+" on "+String(data_0_date)+": "+String(data_0_confirmed_diff));
}
