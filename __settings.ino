/*
   Settings
*/

const String settings_prefix = "/settings_";
const String settings_suffix = ".json";

void startFsSettings() {
  Serial.print("FsSettings starting: ");
  if (SPIFFS.begin()) {
    Serial.println("ok");

    readLedSettings();
    readRtcSettings();
    readSensorsSettings();
    readWifiSettings();
    
  } else {
    Serial.println("failed");
    delay(1000);
    ESP.restart();
  }
}

/*
  LED Settings
*/
struct Settings_led {
  String led_devices;
  String led_start_info;
  
  String led_base_intensity;
  
  String led_dimmer_enabled;
  String led_dimmer_day_hour;
  String led_dimmer_day_minute;
  String led_dimmer_day_intensity;
  String led_dimmer_night_hour;
  String led_dimmer_night_minute;
  String led_dimmer_night_intensity;
} ledSettings;

bool readLedSettings() {
  File file = SPIFFS.open(settings_prefix + "led" + settings_suffix, "r");
  String settings = file.readString();
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, settings);
  if (error) return false;

  ledSettings.led_devices = doc["led_devices"].as<String>();
  ledSettings.led_start_info = doc["led_start_info"].as<String>();
  ledSettings.led_base_intensity = doc["led_base_intensity"].as<String>();
  
  
  ledSettings.led_dimmer_enabled = doc["led_dimmer_enabled"].as<String>();
  ledSettings.led_dimmer_day_hour = doc["led_dimmer_day_hour"].as<String>();
  ledSettings.led_dimmer_day_minute = doc["led_dimmer_day_minute"].as<String>();
  ledSettings.led_dimmer_day_intensity = doc["led_dimmer_day_intensity"].as<String>();
  ledSettings.led_dimmer_night_hour = doc["led_dimmer_night_hour"].as<String>();
  ledSettings.led_dimmer_night_minute = doc["led_dimmer_night_minute"].as<String>();
  ledSettings.led_dimmer_night_intensity = doc["led_dimmer_night_intensity"].as<String>();
    
  return true;
}

bool writeLedSettings() {
  DynamicJsonDocument doc(1024);
  doc["led_devices"] = ledSettings.led_devices;
  doc["led_start_info"] = ledSettings.led_start_info;
  doc["led_base_intensity"] = ledSettings.led_base_intensity;
  
  
  doc["led_dimmer_enabled"] = ledSettings.led_dimmer_enabled;
  doc["led_dimmer_day_hour"] = ledSettings.led_dimmer_day_hour;
  doc["led_dimmer_day_minute"] = ledSettings.led_dimmer_day_minute;
  doc["led_dimmer_day_intensity"] = ledSettings.led_dimmer_day_intensity;
  doc["led_dimmer_night_hour"] = ledSettings.led_dimmer_night_hour;
  doc["led_dimmer_night_minute"] = ledSettings.led_dimmer_night_minute;
  doc["led_dimmer_night_intensity"] = ledSettings.led_dimmer_night_intensity;
  
  File file = SPIFFS.open(settings_prefix + "led" + settings_suffix, "w");
  bool result = serializeJson(doc, file);
  file.close();
  return result;
}

/*
  RTC Settings
*/
struct Settings_rtc {
  String rtc_time_server;
  String rtc_time_gmt;
  String rtc_update_enabled;
  String rtc_update_interval;
} rtcSettings;

bool readRtcSettings() {
  File file = SPIFFS.open(settings_prefix + "rtc" + settings_suffix, "r");
  String settings = file.readString();
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, settings);
  if (error) return false;

  rtcSettings.rtc_time_server = doc["rtc_time_server"].as<String>();
  rtcSettings.rtc_time_gmt = doc["rtc_time_gmt"].as<String>();
  rtcSettings.rtc_update_enabled = doc["rtc_update_enabled"].as<String>();
  rtcSettings.rtc_update_interval = doc["rtc_update_interval"].as<String>();
    
  return true;
}

bool writeRtcSettings() {
  DynamicJsonDocument doc(1024);
  doc["rtc_time_server"] = rtcSettings.rtc_time_server;
  doc["rtc_time_gmt"] = rtcSettings.rtc_time_gmt;
  doc["rtc_update_enabled"] = rtcSettings.rtc_update_enabled;
  doc["rtc_update_interval"] = rtcSettings.rtc_update_interval;
  
  File file = SPIFFS.open(settings_prefix + "rtc" + settings_suffix, "w");
  bool result = serializeJson(doc, file);
  file.close();
  return result;
}


/*
  Sensors Settings
*/
struct Settings_sensors {
  String bmp_local_altitude;
} sensorsSettings;

bool readSensorsSettings() {
  File file = SPIFFS.open(settings_prefix + "sensors" + settings_suffix, "r");
  String settings = file.readString();
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, settings);
  if (error) return false;

  sensorsSettings.bmp_local_altitude = doc["bmp_local_altitude"].as<String>();
    
  return true;
}

bool writeSensorsSettings() {
  DynamicJsonDocument doc(1024);
  doc["bmp_local_altitude"] = sensorsSettings.bmp_local_altitude;
  
  File file = SPIFFS.open(settings_prefix + "sensors" + settings_suffix, "w");
  bool result = serializeJson(doc, file);
  file.close();
  return result;
}


/*
  Wifi Settings
*/
struct Settings_wifi {
  String wifi_start_sta;
  String wifi_ssid;
  String wifi_password;
} wifiSettings;

bool readWifiSettings() {
  File file = SPIFFS.open(settings_prefix + "wifi" + settings_suffix, "r");
  String settings = file.readString();
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, settings);
  if (error) return false;

  wifiSettings.wifi_start_sta = doc["wifi_start_sta"].as<String>();
  wifiSettings.wifi_ssid = doc["wifi_ssid"].as<String>();
  wifiSettings.wifi_password = doc["wifi_password"].as<String>();
  
  return true;
}

bool writeWifiSettings() {
  DynamicJsonDocument doc(1024);
  doc["wifi_start_sta"] = wifiSettings.wifi_start_sta;
  doc["wifi_ssid"] = wifiSettings.wifi_ssid;
  doc["wifi_password"] = wifiSettings.wifi_password;
  
  File file = SPIFFS.open(settings_prefix + "wifi" + settings_suffix, "w");
  bool result = serializeJson(doc, file);
  file.close();
  return result;
}
