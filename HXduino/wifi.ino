#include "default.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

IPAddress apIP(192, 168, 4, 1);             // The IP address of the access point

void startWiFi() {
  WiFi.hostname(WIFI_SSID);
  WiFi.mode(WIFI_AP);
  if(!WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0))){
    ota("AP error");
  }
  if(!WiFi.softAP(WIFI_SSID, "")){
    ota("AP not start");
  }
  //WiFi.softAPIP()
  ota("AP start");
}

void startOTA() { // start the Over The Air update services

  ArduinoOTA.onStart([]() { 
    ota("Update start");
   });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) { 
    ota_prog(progress, total);
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
    while(1);
  });  
  
  ota("OTA start");
  ArduinoOTA.begin();
}

void startMDNS() { // Start the mDNS responder
  MDNS.begin(WIFI_SSID);
  Serial.println("mDNS responder started");
}

void ota_loop(){
    startWiFi();
    startMDNS();
    startOTA();
    while(1){
        ArduinoOTA.handle();
    }
}