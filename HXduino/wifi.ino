#include "default.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
//#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
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
  MDNS.addService("http", "tcp", 80);
  Serial.println(F("mDNS started"));
}

void startHTTP(){
  httpUpdater.setup(&httpServer, "/");
  httpServer.begin();
}

void ota_loop(){
    startWiFi();
    startMDNS();
    ota("Update wait..");
    startHTTP();
    while(1){
        MDNS.update();
        httpServer.handleClient();
    }
}
