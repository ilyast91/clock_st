/*
   WebServer
*/
void startWebServer() {
  Serial.print("WebServer starting: ");
  server.on("/", HTTP_GET, handle_Root);  
  
  server.on("/settings_led", HTTP_GET, handle_settings_led_get);
  server.on("/settings_led", HTTP_POST, handle_settings_led_post);
  server.on("/settings_rtc", HTTP_GET, handle_settings_rtc_get);
  server.on("/settings_rtc", HTTP_POST, handle_settings_rtc_post);
  server.on("/settings_sensors", HTTP_GET, handle_settings_sensors_get);
  server.on("/settings_sensors", HTTP_POST, handle_settings_sensors_post);  
  server.on("/settings_wifi", HTTP_GET, handle_settings_wifi_get);
  server.on("/settings_wifi", HTTP_POST, handle_settings_wifi_post);
  
  server.on("/restart", HTTP_GET, handle_restart);
  server.onNotFound(handle_notFound);

  server.begin();
  Serial.println("ok");
}

void webServerLooping() {
  server.handleClient();
}

/*
   Endpoints
*/
void handle_Root()
{
  String handle_Root_response;

  handle_Root_response += "<div class=\"row\">";
  
  // card 1
  handle_Root_response += "<div class=\"col-xl-4 col-lg-6 col-sm-12\">";
  handle_Root_response += "<div class=\"card m-3\"><div class=\"card-body\">";
  handle_Root_response += "<h5 class=\"card-title\">Часы</h5>";
  handle_Root_response += "<p class=\"card-text\">" + beautifulDateTime(rtc.GetDateTime()) + "</p>";
  handle_Root_response += "</div></div>";
  handle_Root_response += "</div>";

  // card 2
  handle_Root_response += "<div class=\"col-xl-4 col-lg-6 col-sm-12\">";
  handle_Root_response += "<div class=\"card m-3\"><div class=\"card-body\">";
  handle_Root_response += "<h5 class=\"card-title\">Температура (Dallas)</h5><p class=\"card-text\">";
  for (uint8_t i = 0; i < sensors.getDS18Count(); i++) {
    handle_Root_response += "Сенсор, индекс "  + (String) i + ": " + dallasReadTemp(i) + "</br>";
  }
  handle_Root_response += "</p></div></div>";
  handle_Root_response += "</div>";

  // card 3
  handle_Root_response += "<div class=\"col-xl-4 col-lg-6 col-sm-12\">";
  handle_Root_response += "<div class=\"card m-3\"><div class=\"card-body\">";
  handle_Root_response += "<h5 class=\"card-title\">BPM280</h5><p class=\"card-text\">";
  handle_Root_response += "Температура: " + (String) bmpReadTemperature() + "</br>";
  handle_Root_response += "Давление: " + (String) bmpReadPressure() + "</br>";
  handle_Root_response += "Высота: " + (String) bmpReadAltitude();
  handle_Root_response += "</p></div></div>";
  handle_Root_response += "</div>";
  
  // card 4
  handle_Root_response += "<div class=\"col-xl-4 col-lg-6 col-sm-12\">";
  handle_Root_response += "<div class=\"card m-3\"><div class=\"card-body\">";
  handle_Root_response += "<h5 class=\"card-title\">Устройства LED</h5>";
  for (int device = 0; device < staticLedSettings.size(); device++) {
    handle_Root_response += "<p class=\"card-text\">Индекс: " + String(device) + "</p>";
    handle_Root_response += "<p class=\"card-text\"><table class=\"table table-sm table-hover table-bordered\"><tbody>";
    for (JsonPair kv : staticLedSettings[device].as<JsonObject>() ) {
      String key = String(kv.key().c_str());
      if (key == "sources_left" || key == "sources_right") {
        handle_Root_response += "<tr><td><strong>" + String(kv.key().c_str()) + "</td><td>";
        for (int i = 0; i < staticLedSettings[device][key].size(); i++) {
          String source = staticLedSettings[device][key][i]["source"].as<String>();
          String interval = staticLedSettings[device][key][i]["interval"].as<String>();
          handle_Root_response += "<strong>" + source + "</strong>: " + interval + " <br>";
        }
        handle_Root_response += "</td></tr>";
      } else {
        handle_Root_response += "<tr><td><strong>" + String(kv.key().c_str()) + "</strong></td><td>" + kv.value().as<String>() + "</td></tr>";
      }
    }
    handle_Root_response += "</tbody></table></p>";
  }
  handle_Root_response += "</div></div>";
  handle_Root_response += "</div>";
  
  // card 5
  handle_Root_response += "<div class=\"col-xl-4 col-lg-6 col-sm-12\">";
  handle_Root_response += "<div class=\"card m-3\"><div class=\"card-body\">";
  handle_Root_response += "<h5 class=\"card-title\">Информация о плате</h5>";
  handle_Root_response += "<p class=\"card-text\"><table class=\"table table-sm table-hover table-bordered\"><tbody>";  
  handle_Root_response += "<tr><td><strong>Vcc</strong></td><td>"+String(ESP.getVcc())+"</td></tr>";
  handle_Root_response += "<tr><td><strong>FreeHeap</strong></td><td>"+String(ESP.getFreeHeap())+"</td></tr>";  
  handle_Root_response += "<tr><td><strong>ChipId</strong></td><td>"+String(ESP.getChipId())+"</td></tr>";
  handle_Root_response += "<tr><td><strong>SdkVersion</strong></td><td>"+String(ESP.getSdkVersion())+"</td></tr>";
  handle_Root_response += "<tr><td><strong>BootVersion</strong></td><td>"+String(ESP.getBootVersion())+"</td></tr>";
  handle_Root_response += "<tr><td><strong>BootMode</strong></td><td>"+String(ESP.getBootMode())+"</td></tr>";
  handle_Root_response += "<tr><td><strong>CpuFreqMHz</strong></td><td>"+String(ESP.getCpuFreqMHz())+"</td></tr>";
  handle_Root_response += "<tr><td><strong>FlashChipId</strong></td><td>"+String(ESP.getFlashChipId())+"</td></tr>";
  handle_Root_response += "<tr><td><strong>FlashChipRealSize</strong></td><td>"+String(ESP.getFlashChipRealSize())+"</td></tr>";
  handle_Root_response += "<tr><td><strong>FlashChipSpeed</strong></td><td>"+String(ESP.getFlashChipSpeed())+"</td></tr>";
  handle_Root_response += "<tr><td><strong>FlashChipMode</strong></td><td>"+String(ESP.getFlashChipMode())+"</td></tr>";
  handle_Root_response += "<tr><td><strong>FlashChipSizeByChipId</strong></td><td>"+String(ESP.getFlashChipSizeByChipId())+"</td></tr>";
  handle_Root_response += "<tr><td><strong>SketchSize</strong></td><td>"+String(ESP.getSketchSize())+"</td></tr>";
  handle_Root_response += "<tr><td><strong>FreeSketchSpace</strong></td><td>"+String(ESP.getFreeSketchSpace())+"</td></tr>";
  handle_Root_response += "<tr><td><strong>ResetInfo</strong></td><td>"+String(ESP.getResetInfo())+"</td></tr>";
  handle_Root_response += "<tr><td><strong>CycleCount</strong></td><td>"+String(ESP.getCycleCount())+"</td></tr>";
  handle_Root_response += "</tbody></table></p>";
  handle_Root_response += "</div></div>";
  handle_Root_response += "</div>";

  // card 6
  handle_Root_response += "<div class=\"col-xl-4 col-lg-6 col-sm-12\">";
  handle_Root_response += "<div class=\"card m-3\"><div class=\"card-body\">";
  handle_Root_response += "<h5 class=\"card-title\">Список I2C устройств</h5><p class=\"card-text\">";
  for (byte address = 8; address < 127; address++ ) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();
    if (error == 0) {
      handle_Root_response += "I2C устройство найдено по адресу: 0x" + String(address, HEX) + "</br>";
    } else if (error == 4) {
      handle_Root_response += "Необьяснимая ошибка устройства по адресу: 0x" + String(address, HEX) + "</br>";
    }
  }
  handle_Root_response += "</p></div></div>";
  handle_Root_response += "</div>";
  
  handle_Root_response += "</div>";
  
  sendHTML(200, handle_Root_response);
}


/* 
 Led Settings
*/
void handle_settings_led_get() {  
  String settings_form;
  
  settings_form += "<form method=\"post\"><legend>Изменение настроек LED</legend>";
  settings_form += createYesNoSelect("led_start_info", "Отображать индекс дисплея при старте", ledSettings.led_start_info);

  settings_form += "<h5 class=\"mt-3\">Автоматическое изменение яркости</h5>";  
  settings_form += "<div class=\"row\">";
  settings_form += createFormTextElement("led_base_intensity", "Базовая яркость", ledSettings.led_base_intensity);    
  settings_form += createYesNoSelect("led_dimmer_enabled", "Автоматическое изменение яркости, вкл", ledSettings.led_dimmer_enabled); 
  settings_form += "</div>";
  settings_form += "<div class=\"row\">";
  settings_form += createFormTextElement("led_dimmer_day_hour", "День часы", ledSettings.led_dimmer_day_hour);         
  settings_form += createFormTextElement("led_dimmer_day_minute", "День минуты", ledSettings.led_dimmer_day_minute);
  settings_form += createIntensitySelect("led_dimmer_day_intensity", "День яркость", ledSettings.led_dimmer_day_intensity);  
  settings_form += "</div>";
  settings_form += "<div class=\"row\">";
  settings_form += createFormTextElement("led_dimmer_night_hour", "Ночь часы", ledSettings.led_dimmer_night_hour);
  settings_form += createFormTextElement("led_dimmer_night_minute", "Ночь минуты", ledSettings.led_dimmer_night_minute);
  settings_form += createIntensitySelect("led_dimmer_night_intensity", "Ночь Яркость", ledSettings.led_dimmer_night_intensity);  
  settings_form += "</div>";
  
  settings_form += "<h5 class=\"mt-3\">Изменение отображения данных на дисплеях</h5>";  
  settings_form += "<div class=\"mb-3\">";
  settings_form += "<textarea class=\"form-control\" id=\"led_devices\" rows=\"10\" name=\"led_devices\" style=\"display:none;\">" + staticLedSettings.as<String>() + "</textarea>";
  settings_form += "<div id=\"led_devices_form\"></div>";  
  settings_form += "</div>";
  settings_form += "<button type=\"submit\" class=\"btn btn-primary mx-1\">Изменить</button>";
  settings_form += "</form>";

  sendHTML(200, "LED", settings_form);
}

void handle_settings_led_post() {
  ledSettings.led_devices = server.arg("led_devices");
  ledSettings.led_start_info = server.arg("led_start_info");
  ledSettings.led_base_intensity = server.arg("led_base_intensity"); 
  
  ledSettings.led_dimmer_enabled = server.arg("led_dimmer_enabled");
  ledSettings.led_dimmer_day_hour = server.arg("led_dimmer_day_hour");
  ledSettings.led_dimmer_day_minute = server.arg("led_dimmer_day_minute");
  ledSettings.led_dimmer_day_intensity = server.arg("led_dimmer_day_intensity");
  ledSettings.led_dimmer_night_hour = server.arg("led_dimmer_night_hour");
  ledSettings.led_dimmer_night_minute = server.arg("led_dimmer_night_minute");
  ledSettings.led_dimmer_night_intensity = server.arg("led_dimmer_night_intensity");
  
  if (!writeLedSettings()) {
    sendHTML(200, "<div class=\"alert alert-danger\" role=\"alert\">Ошибка обновления настроек</div>");
  }

  updateStaticLedSettings();

  server.sendHeader("Location", String("/settings_led"), true);
  server.send(302, "text/plain", "");
}

/* 
 Clock Settings
*/
void handle_settings_rtc_get() {
  RtcDateTime dt = rtc.GetDateTime();
  
  String settings_form;
  
  settings_form += "<form method=\"post\" class=\"mb-5\">";
  
  settings_form += "<h3>Текущее время: " + beautifulDateTime(dt) + "</h5>";

  settings_form += "<h5 class=\"mt-3\">Обновить из интернета принудительно</h5>";  
  settings_form += "<button type=\"submit\" class=\"btn btn-primary mx-1\" name=\"action\" value=\"internet\">Обновить</button>";
  
  settings_form += "<h5 class=\"mt-3\">Изменение настроек часов</h5>";  
  settings_form += "<legend></legend>";
  settings_form += "<div class=\"row\">";
  settings_form += createFormTextElement("rtc_time_server", "Сервер обновления", rtcSettings.rtc_time_server);
  settings_form += createFormTextElement("rtc_time_gmt", "GMT", rtcSettings.rtc_time_gmt);
  settings_form += "</div>";
  settings_form += "<div class=\"row\">";  
  settings_form += createYesNoSelect("rtc_update_enabled", "Автоматическое обновление, вкл", rtcSettings.rtc_update_enabled);  
  settings_form += createFormTextElement("rtc_update_interval", "Интервал обновления, мин", rtcSettings.rtc_update_interval);
  settings_form += "</div>";
  settings_form += "<button type=\"submit\" class=\"btn btn-primary mx-1\" name=\"action\" value=\"rtc_settings\">Изменить</button>";
  
  settings_form += "<h5 class=\"mt-3\">Изменение настроек часов вручную</h5>";
  settings_form += "<div class=\"row\">";
  settings_form += createFormTextElement("manual_year", "Год", String(dt.Year(), DEC));
  settings_form += createFormTextElement("manual_month", "Месяц", String(dt.Month(), DEC));
  settings_form += createFormTextElement("manual_day", "День", String(dt.Day(), DEC));
  settings_form += "</div>";
  settings_form += "<div class=\"row\">";
  settings_form += createFormTextElement("manual_hour", "Часы", String(dt.Hour(), DEC));
  settings_form += createFormTextElement("manual_minute", "Минуты", String(dt.Minute(), DEC));
  settings_form += createFormTextElement("manual_seconds", "Секунды", String(dt.Second(), DEC));
  settings_form += "</div>";
  settings_form += "<button type=\"submit\" class=\"btn btn-primary mx-1\" name=\"action\" value=\"manual_clock\">Изменить</button>";
  settings_form += "</form>";

  sendHTML(200, "Часы", settings_form);
}

void handle_settings_rtc_post() {
  String action = server.arg("action");

  if (action == "rtc_settings") {
    rtcSettings.rtc_time_server = server.arg("rtc_time_server");
    rtcSettings.rtc_time_gmt = server.arg("rtc_time_gmt");
    rtcSettings.rtc_update_enabled = server.arg("rtc_update_enabled");
    rtcSettings.rtc_update_interval = server.arg("rtc_update_interval");
    if (!writeRtcSettings()) {
      sendHTML(200, "<div class=\"alert alert-danger\" role=\"alert\">Ошибка обновления настроек</div>");
    }
  } 
  if (action == "manual_clock") {
    char manual_year[20];
    server.arg("manual_year").toCharArray(manual_year, 20);
    char manual_month[20];
    server.arg("manual_month").toCharArray(manual_month, 20);
    char manual_day[20];
    server.arg("manual_day").toCharArray(manual_day, 20);
    char manual_hour[20];
    server.arg("manual_hour").toCharArray(manual_hour, 20);
    char manual_minute[20];
    server.arg("manual_minute").toCharArray(manual_minute, 20);
    char manual_seconds[20];
    server.arg("manual_seconds").toCharArray(manual_seconds, 20);
    RtcDateTime currentTime = RtcDateTime(atoi(manual_year), atoi(manual_month), atoi(manual_day), atoi(manual_hour), atoi(manual_minute), atoi(manual_seconds));
    rtc.SetDateTime(currentTime);
  }
  if (action == "internet") {
    updateDateTimeFromInternet();
  }
  setBaseIntensity();
  server.sendHeader("Location", String("/settings_rtc"), true);
  server.send(302, "text/plain", "");
}

/*
  Sensors Settings
*/
void handle_settings_sensors_get() {

  String settings_form;
  settings_form += "<form method=\"post\"><legend>Изменение настроек сенсоров</legend>";
  settings_form += createFormTextElement("bmp_local_altitude", "Давление на уровне моря, гПа", sensorsSettings.bmp_local_altitude);
  settings_form += "<button type=\"submit\" class=\"btn btn-primary mx-1\">Изменить</button>";
  settings_form += "</form>";

  sendHTML(200, "Сенсоры", settings_form);
}

void handle_settings_sensors_post() {
  sensorsSettings.bmp_local_altitude = server.arg("bmp_local_altitude");
  if (!writeSensorsSettings()) {
    sendHTML(200, "<div class=\"alert alert-danger\" role=\"alert\">Ошибка обновления настроек</div>");
  }
  server.sendHeader("Location", String("/settings_sensors"), true);
  server.send(302, "text/plain", "");
}

/*
  Wifi Settings
*/
void handle_settings_wifi_get() {

  String settings_form;
  settings_form += "<form method=\"post\"><legend>Изменение настроек WIFI</legend>";
  settings_form += createYesNoSelect("wifi_start_sta", "Wifi mode (Да - SPA, Нет - AP)", wifiSettings.wifi_start_sta);  
  settings_form += createFormTextElement("wifi_ssid", "SSID", wifiSettings.wifi_ssid);
  settings_form += createFormTextElement("wifi_password", "Password", wifiSettings.wifi_password);
  settings_form += "<button type=\"submit\" class=\"btn btn-primary mx-1\">Изменить</button>";
  settings_form += "</form>";

  sendHTML(200, "WIFI", settings_form);
}

void handle_settings_wifi_post() {
  wifiSettings.wifi_start_sta = server.arg("wifi_start_sta");
  wifiSettings.wifi_ssid = server.arg("wifi_ssid");
  wifiSettings.wifi_password = server.arg("wifi_password");
  if (!writeWifiSettings()) {
    sendHTML(200, "<div class=\"alert alert-danger\" role=\"alert\">Ошибка обновления настроек</div>");
  }
  server.sendHeader("Location", String("/settings_wifi"), true);
  server.send(302, "text/plain", "");
}

/*
  Common
*/
void handle_restart() {
  sendHTML(200, "Перезагрузка", "Выполнено <br> Устройство перезагрузится в течении нескольких секунд.");
  delay(3000);
  ESP.restart();
}

void handle_notFound() {
  sendHTML(404, "Страница не найдена");
}
