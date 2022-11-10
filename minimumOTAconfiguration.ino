/*
    This sketch performs the minimum configuration for OTA through webserver and basic OTA through Arduino IDE.
    The OTA updates through webserver are performed when deploying the limboesp git-hub repository: https://github.com/thomaskuh/limboesp.
    The sketch is also compatible for ESP32 and ESP8266 boards.
 */

#include "config.h"

void setup() {
 Serial.begin(9600);
 start_wifi();
 otaConfig();
 checkOTAUpdates();
}

void loop() {
  // Keep both function below always within the loop to have periodic checks
  ArduinoOTA.handle();
  checkOTAUpdates();
}

void start_wifi() {
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(WLAN_SSID, WLAN_PWD); 
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  
}

// Function to check OTA updates through web server
void checkOTAUpdates() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClientSecure client_ota;
    client_ota.setInsecure();
    client_ota.setTimeout(12000 / 1000); // timeout argument is defined in seconds for setTimeout
    
    t_httpUpdate_return ret = http_update.update(client_ota, OTA_URL, OPT_OTA_MSG);
  
    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", http_update.getLastError(), http_update.getLastErrorString().c_str());
        break;
  
      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;
  
      case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    }
  }
}

// Function to send OTA update through local network
void otaConfig(){
// Port defaults to 3232
// ArduinoOTA.setPort(3232);

// Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname(OTA_NAME);

// No authentication by default
  ArduinoOTA.setPassword(OTA_PWD);

// Password can be set with it's md5 value as well
// MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
//  ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
}
