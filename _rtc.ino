/*
   RTC DS3231
*/

long rtc_update_time = 0;

void startRTC() {
  Serial.print("RTC starting: ");
  rtc.Begin();
  Serial.println("ok");
}

void rtcLooping() {
  if (rtcSettings.rtc_update_enabled == "true") {
    long diff = millis() - rtc_update_time;
    int interval = rtcSettings.rtc_update_interval.toInt();
    if (diff > interval * 60000) {
      updateDateTimeFromInternet();
      rtc_update_time = millis();
    }
  }
}

void updateDateTimeFromInternet() {
  String timeServer = rtcSettings.rtc_time_server;
  char charServer[99];
  timeServer.toCharArray(charServer, 99);

  int timeOffset = rtcSettings.rtc_time_gmt.toInt() * 3600;

  timeClient.setPoolServerName(charServer);
  timeClient.setTimeOffset(timeOffset);
  
  bool result = timeClient.update();
  long epoch = timeClient.getEpochTime();
  epoch -= 946684800;
  if (result && epoch > 0) {
    RtcDateTime currentTime = RtcDateTime(epoch);
    rtc.SetDateTime(currentTime);
  }
}

String beautifulDateTime(RtcDateTime dt) {
  String date = String(dt.Year(), DEC) + "-" + beautyTimeUnit(String(dt.Month(), DEC)) + "-" + beautyTimeUnit(String(dt.Day(), DEC));
  String times = beautyTimeUnit(String(dt.Hour(), DEC)) + ":" + beautyTimeUnit(String(dt.Minute(), DEC)) + ":" + beautyTimeUnit(String(dt.Second(), DEC));
  return date + " " + times;
}

String beautyTimeUnit(String unit) {
  if (unit.length() < 2) unit = "0" + unit;
  return unit;
}
