#include <LiquidCrystal.h> //including library to use LCD
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Time.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#ifndef STASSID
#define STASSID "YOUR WIFI NAME GOES HERE" // MAKE SURE TO CHANGE THIS BEFORE PUSHING THE CODE TO THE ESP8266 !!!
#define STAPSK  "YOUR WIFI PASSWORD GOES HERE" // MAKE SURE TO CHANGE THIS BEFORE PUSHING THE CODE TO THE ESP8266 !!!
#endif

int i = 0;
int j = 0;
int timezone = -5;
int dst = 3600;
const char* ssid = STASSID;
const char* password = STAPSK;
const char* host = "weather-ota";

LiquidCrystal lcd(D2, D3, D5, D6, D7, D8); //mapping LCD pins to ESP's pins
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  lcd.begin(16, 2);
  Serial.print("Connecting ...");
  lcd.print("Connecting...");
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    lcd.setCursor(i, 1);
    lcd.print(">");
    i++;
    delay(100);
  }
  i = 0;

  MDNS.begin(host);
  httpUpdater.setup(&httpServer);
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
  Serial.println("");
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
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
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  lcd.setCursor(0, 0);
  configTime((timezone * 3600), dst, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time ...");
  lcd.print("Syncing Time...");
  while (!time(nullptr)) {
    lcd.setCursor(j, 1);
    lcd.print(">");
    j++;
    delay(100);
  }
  j = 0;
}

void loop() {
  ArduinoOTA.handle();
  httpServer.handleClient();
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;  //Object of class HTTPClient
    http.begin("http://openweathermap.org/data/2.5/weather/?appid=API-ID-GOES-HERE&id=5906752&units=metric"); // MAKE SURE YOU CHANGE THIS TO YOUR API ID 
    int httpCode = http.GET();
    
    //Check the returning code                                                                  
    if (httpCode > 0) {
      const size_t bufferSize = 2*JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(2) + 4*JSON_OBJECT_SIZE(1) + 3*JSON_OBJECT_SIZE(2) + 3*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + 2*JSON_OBJECT_SIZE(7) + 2*JSON_OBJECT_SIZE(8) + 720;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
      if (!root.success()) {
        Serial.println("parseObject() failed");
        lcd.print("SERVER ERROR");
        return;
      }

      char buff[100];
      time_t now = time(nullptr);
      strftime (buff, 100, "%Y/%m/%d %H:%M", localtime (&now));
      float temperature = root["main"]["temp"];
      int humidity = root["main"]["humidity"];
      String descr = root["weather"][0]["main"];
//    Serial.printf("(OTTAWA, ON) Temperature : %d °C\n", round(temperature));
//    Serial.printf("Humidity : %d%\n", humidity);
      lcd.clear();
      lcd.print((int)round(temperature));
      lcd.print(char(176));
      lcd.print("C ");
      lcd.print(humidity);
      lcd.print("% ");
      lcd.print(descr);
      lcd.setCursor(0, 1);
      lcd.print(buff);
      lcd.setCursor(0, 0);
    }
    http.end();   //Close connection
  }
}
