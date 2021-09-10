#include "default.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#ifdef USE_OTA
#include <ArduinoOTA.h>
#else
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#endif

IPAddress apIP(192, 168, 4, 1);             // The IP address of the access point

void startWiFi() {
  WiFi.hostname(WIFI_SSID);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(WIFI_SSID, "");
  otaf(PSTR("AP start"));
}

void startMDNS() { // Start the mDNS responder
  MDNS.begin(WIFI_SSID);
  Serial.println(F("mDNS responder started"));
}

#ifdef USE_OTA
void startOTA() { // start the Over The Air update services
  ArduinoOTA.onStart([]() {
    if(ArduinoOTA.getCommand() == U_FLASH) 
      ota("Update start: fw");
    else
      ota("Update start: fs");
    Serial.print("free sketch size=");
    Serial.println(ESP.getFreeSketchSpace());
   });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) { 
    ota_prog(progress, total);
    yield();
  });
  
  ArduinoOTA.onEnd([]() { 
    ota("Update End");
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    if (error == OTA_AUTH_ERROR) 
        ota("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) 
        ota("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
        ota("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
        ota("Receive Failed");
    else if (error == OTA_END_ERROR)
        ota("End Failed");
    while(1)yield();
  });  
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname(WIFI_SSID);
  ota("OTA wait..");
  ArduinoOTA.begin();
}
#else
void update_started() {
    otaf(PSTR("Update start"));
    Serial.print(F("free sketch size="));
    Serial.println(ESP.getFreeSketchSpace());
}

void update_finished() {
    ota("Update End");
}

void update_progress(int progress, int total) {
    ota_prog(progress, total);
    yield();
}

void update_error(int err) {
    ota("End Failed");
    Serial.printf(PSTR("HTTP update error %d\n"), err);
    while(1)yield();
}

void startHTTP(){
    WiFiClient client;
    if(client){
        ota("Connected..");
        ESPhttpUpdate.onStart(update_started);
        ESPhttpUpdate.onEnd(update_finished);
        ESPhttpUpdate.onProgress(update_progress);
        ESPhttpUpdate.onError(update_error);
        t_httpUpdate_return ret = ESPhttpUpdate.update(client, "192.168.4.1", 80, "HXduino.ino.bin");
        switch (ret) {
          case HTTP_UPDATE_FAILED:
            ota("Update fail");
            Serial.printf(PSTR("HTTP_UPDATE Error (%d): %s\n"), ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            break;
          case HTTP_UPDATE_NO_UPDATES:
            ota("No Update");
            break;
          case HTTP_UPDATE_OK:
            ota("Update end");
            break;
        }
    }
}
#endif

void ota_loop(){
    startWiFi();
    startMDNS();
#ifdef USE_OTA
    startOTA();
    while(1){
      yield();
      ArduinoOTA.handle();
    }
#else
    ota("Connect wait..");
    while(1){
        yield();
        startHTTP();
        delay(500);
    }
#endif
}
