/*
   Clock ST
*/

#include <Wire.h>
#include <FS.h>
#include <ArduinoJson.h>            // ArduinoJson
#include <RtcDS3231.h>              // Rtc by Makuna
#include <OneWire.h>
#include <DallasTemperature.h>      // DallasTemperature
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>              // NTPClient
#include <ESP8266WebServer.h>
#include <LedControl.h>             // LedControl    
#include <Adafruit_BME280.h>        // Adafruit    
#include <Adafruit_BMP280.h>                
#include <Adafruit_AHTX0.h>

/*
   PIN configurations
*/
#define RTC_DS3231_SCL D1
#define RTC_DS3231_SDA D2
#define DALLAS_PIN D4
#define LED_CTL_CLK_PIN D5
#define LED_CTL_DATA_PIN D6
#define LED_CTL_CS_PIN D7
#define RESET_PIN D0

#define WEB_SERVER_PORT 80

RtcDS3231<TwoWire> rtc(Wire);
OneWire oneWire(DALLAS_PIN);
DallasTemperature sensors(&oneWire);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
ESP8266WebServer server(WEB_SERVER_PORT);
LedControl lc = LedControl(LED_CTL_DATA_PIN, LED_CTL_CLK_PIN, LED_CTL_CS_PIN, 8);
Adafruit_BME280 bme280;
Adafruit_BMP280 bmp280;
Adafruit_AHTX0 aht10;

bool safeMode = false;

/*
   Initialization
*/
void setup() {
  Serial.begin(9600);
  Serial.setDebugOutput(false);
  Serial.println("\nStarting");
  pinMode(RESET_PIN, INPUT);
  checkPressedOnStartSafeModeBtn();
  startFsSettings();
  startRTC();
  startLedControl();
  startSensors();
  startWiFi();
  startWebServer();
}

/*
   Loop
*/
void loop() {
  webServerLooping();
  ledControlLooping();
  checkPressedSafeModeBtn();
  rtcLooping();
  dimmerLooping();
}
