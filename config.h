#if defined(ESP32)
  #define BOARD "esp32"
  #include <WiFi.h>
  #include <WiFiMulti.h>
  #include <HTTPUpdate.h>
  HTTPUpdate http_update;
  WiFiMulti WiFiMulti;
#endif

#if defined(ESP8266)
  #define BOARD "esp8266"
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <ESP8266WiFiMulti.h>
  #include <ESP8266httpUpdate.h>
  ESP8266HTTPUpdate http_update;
  ESP8266WiFiMulti WiFiMulti;
#endif

#include <ArduinoOTA.h>

#define WLAN_SSID                         "my-ssid" 
#define WLAN_PWD                          "my-wlan-password"

// The OTA web server is based on the following limboesp git-hub repository: https://github.com/thomaskuh/limboesp

#define OTA_NAME                          "my-board-name"
#define OTA_PWD                           "my-ota-password"
#define OTA_SERVER                        "my-ota-server-name"      //If connection refused, specify the absolute IP address
#define OTA_URL                           OTA_SERVER"/update/"BOARD
#define OPT_OTA_MSG                       OTA_NAME

// The SAMPLING variable (ms) is defined for data aquisition coming from attached sensors
#define SAMPLING                          1000
