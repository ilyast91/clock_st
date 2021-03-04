/*
   7 Digits module
*/

DynamicJsonDocument staticLedSettings(4096);

unsigned long rtc_clock_blinker_time = 0;

bool dimmerUpdated = false;
bool isNight = false;

void startLedControl() {
  Serial.print("LedControl starting: ");
  int devices = lc.getDeviceCount();
  for (int i = 0; i < devices; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 10);
    if (ledSettings.led_start_info == "true") printLedDevicesAddress(i);
  }
  delay(1200);
  updateStaticLedSettings();
  Serial.println("ok");
}

void ledControlLooping() {
  for (int device = 0; device < staticLedSettings.size(); device++) {

    // digits (left)
    if (!staticLedSettings[device]["sources_left"].isNull()) {
      int current_source_left = staticLedSettings[device]["curr_src_left"].as<int>();

      String sensorName_left = staticLedSettings[device]["sources_left"][current_source_left]["source"].as<String>();
      int sensorInterval_left = staticLedSettings[device]["sources_left"][current_source_left]["interval"].as<int>();

      if (sensorInterval_left > 0) {
        long update_time_left = staticLedSettings[device]["upd_left"].as<long>();

        if (update_time_left <= 0) {
          update_time_left = millis();
          staticLedSettings[device]["upd_left"] = update_time_left;
        }

        long passed_left = millis() - update_time_left;
        if (passed_left >= sensorInterval_left * 1000) {
          current_source_left++;
          if (current_source_left >= staticLedSettings[device]["sources_left"].size()) {
            current_source_left = 0;
          }
          staticLedSettings[device]["upd_left"] = millis();
        }
      }
      staticLedSettings[device]["curr_src_left"] = current_source_left;
      switch_print(sensorName_left, device, 7);
    }

    // digits (right)
    if (!staticLedSettings[device]["sources_right"].isNull()) {
      int current_source_right = staticLedSettings[device]["curr_src_right"].as<int>();

      String sensorName_right = staticLedSettings[device]["sources_right"][current_source_right]["source"].as<String>();
      int sensorInterval_right = staticLedSettings[device]["sources_right"][current_source_right]["interval"].as<int>();

      if (sensorInterval_right > 0) {
        long update_time_right = staticLedSettings[device]["upd_right"].as<long>();

        if (update_time_right <= 0) {
          update_time_right = millis();
          staticLedSettings[device]["upd_right"] = update_time_right;
        }

        long passed_right = millis() - update_time_right;
        if (passed_right >= sensorInterval_right * 1000) {
          current_source_right++;
          if (current_source_right >= staticLedSettings[device]["sources_right"].size()) current_source_right = 0;
          staticLedSettings[device]["upd_right"] = millis();
        }
      }
      staticLedSettings[device]["curr_src_right"] = current_source_right;
      switch_print(sensorName_right, device, 3);
    }
  }
}

void dimmerLooping() {
  if (ledSettings.led_dimmer_enabled == "true") {
    RtcDateTime cur_date = rtc.GetDateTime();
    
    int time_curr = (cur_date.Hour() * 60) + cur_date.Minute();
    int time_day = (ledSettings.led_dimmer_day_hour.toInt() * 60) + ledSettings.led_dimmer_day_minute.toInt(); 
    int time_night = (ledSettings.led_dimmer_night_hour.toInt() * 60) + ledSettings.led_dimmer_night_minute.toInt(); 

    bool current_isNight = !(time_day <= time_curr && time_curr < time_night);
    
    if (isNight != current_isNight) {
      isNight = current_isNight;
      dimmerUpdated = false;
    }
    
    if (isNight && !dimmerUpdated) {
      updateIntensity(ledSettings.led_dimmer_night_intensity.toInt());
      dimmerUpdated = true;
    } else if (!isNight && !dimmerUpdated) {
      updateIntensity(ledSettings.led_dimmer_day_intensity.toInt());
      dimmerUpdated = true;
    }
  }
}

void switch_print(String sensor, int deviceIndex, int startLedIndex) {

  if (sensor == "time") print_time(deviceIndex, startLedIndex);
  if (sensor == "date") print_date(deviceIndex, startLedIndex);
  if (sensor == "year") print_year(deviceIndex, startLedIndex);

  if (sensor == "bmp_temp") print_bmp_temp(deviceIndex, startLedIndex);
  if (sensor == "bmp_pressure") print_bmp_pressure(deviceIndex, startLedIndex);
  if (sensor == "bmp_altitude") print_bmp_altitude(deviceIndex, startLedIndex);

  if (sensor.indexOf("temp_") != -1) {
    String strIndex = String(sensor[5]);
    int tempIndex = strIndex.toInt();
    if (tempIndex >= 0) {
      print_temp(deviceIndex, startLedIndex, tempIndex);
    }
  }
}

void print_time(int deviceIndex, int startLedIndex) {
  RtcDateTime dt = rtc.GetDateTime();
  unsigned long currentMillis = millis();
  String currentTime;
  currentTime += beautyTimeUnit(String(dt.Hour(), DEC));
  if (currentMillis - rtc_clock_blinker_time >= 1000) {
    currentTime += ".";
  }
  if (currentMillis - rtc_clock_blinker_time >= 2000) {
    rtc_clock_blinker_time = currentMillis;    
  }
  currentTime += beautyTimeUnit(String(dt.Minute(), DEC));
  print_to_lc_2(currentTime, deviceIndex, startLedIndex);
}

void print_year(int deviceIndex, int startLedIndex) {
  RtcDateTime dt = rtc.GetDateTime();
  String currentDate = String(dt.Year(), DEC);
  print_to_lc_2(currentDate, deviceIndex, startLedIndex);
}

void print_date(int deviceIndex, int startLedIndex) {
  RtcDateTime dt = rtc.GetDateTime();
  String currentDate = beautyTimeUnit(String(dt.Day(), DEC)) + "." + beautyTimeUnit(String(dt.Month(), DEC)) + ".";
  print_to_lc_2(currentDate, deviceIndex, startLedIndex);
}

void print_temp(int deviceIndex, int startLedIndex, int sensorIndex) {
  float temp = dallasReadTemp(sensorIndex);
  int precision = (temp > 99.9 && temp < -99.9 ? 0 : 1);
  print_to_lc_2(String(dallasReadTemp(sensorIndex), precision), deviceIndex, startLedIndex);
}

void print_bmp_temp(int deviceIndex, int startLedIndex) {
  print_to_lc_2(String(bmpReadTemperature(), 1), deviceIndex, startLedIndex);
}

void print_bmp_pressure(int deviceIndex, int startLedIndex) {
  print_to_lc_2(String(bmpReadPressure(), 0), deviceIndex, startLedIndex);
}

void print_bmp_altitude(int deviceIndex, int startLedIndex) {
  print_to_lc_2(String(bmpReadAltitude(), 0), deviceIndex, startLedIndex);
}

void print_to_lc_2(String text, int deviceIndex, int startLedIndex) {
  int dotCount = 0;
  int sizeOfText = text.length();
  for (int i = 0; i < sizeOfText; i++) {
    if (text[i] == '.') {
      dotCount++;
    }
  }
  for (int i = 0; i < 4 - (sizeOfText - dotCount); i++) {
    text = " " + text;
  }
  sizeOfText = text.length();
  dotCount = 0;
  bool dot = false;
  for (int i = 0; i < sizeOfText; i++) {
    if (text[i] == '.') {
      dotCount++;
    } else {
      if (text[i + 1] == '.') dot = true;
      lc.setChar(deviceIndex, startLedIndex - i + dotCount, text[i], dot);
      if (dot) dot = false;
    }
  }
}

void updateStaticLedSettings() {
  deserializeJson(staticLedSettings, ledSettings.led_devices);
  setBaseIntensity();
}

void printLedControlIp(IPAddress ipAddress) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < lc.getDeviceCount(); j++) {
      print_to_lc_2(String(ipAddress[i], DEC), j, 3);
      print_to_lc_2(String(ipAddress[i], DEC), j, 7);
    }
    delay(1200);
  }
  clearDevices();
}

void printLedDevicesAddress(int addr) {
  lc.clearDisplay(addr);
  print_to_lc_2("LEd" + String(addr), addr, 3);
  print_to_lc_2("LEd" + String(addr), addr, 7);
}

void clearDevices() {
  for (int i = 0; i < lc.getDeviceCount(); i++) {
    lc.clearDisplay(i);
  }
}

void setBaseIntensity() {
  updateIntensity(ledSettings.led_base_intensity.toInt());
  dimmerUpdated = false;
}

void updateIntensity(int intensity) {
  int devices = lc.getDeviceCount();
  for (int i = 0; i < devices; i++) {
    if (staticLedSettings.size() > 0) {
      intensity += staticLedSettings[i]["corr_intensity"].as<int>();      
    }
    if (intensity < 0) intensity = 0;
    if (intensity > 15) intensity = 15;
    lc.setIntensity(i, intensity);
  }
  clearDevices();
}
