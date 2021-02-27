/*
   WiFi
*/

String wifi_mode = "AP";
String default_wifi_ssid = "clock_st";
String default_wifi_pass = "12345678";
int connect_timeout = 15000;

void startWiFi() {
  boolean result = false;
  WiFi.softAPdisconnect (true);
  WiFi.mode(WIFI_OFF);
  if (safeMode) {
    result = startWiFiApMode(default_wifi_ssid, default_wifi_pass);
  } else {
    if (wifiSettings.wifi_start_sta == "true" && wifiSettings.wifi_ssid.length() > 0 && wifiSettings.wifi_password.length() > 0) {
      result = startWiFiStaMode(wifiSettings.wifi_ssid, wifiSettings.wifi_password);
    }else {
      result = startWiFiApMode(wifiSettings.wifi_ssid, wifiSettings.wifi_password);      
    }
  }
  Serial.println("WiFi starting: " + String(result ? "ok" : "failed"));
}

bool startWiFiApMode(String ssid, String pass) {
  if (ssid.length() <= 0) ssid = default_wifi_ssid;
  if (pass.length() <= 0) pass = default_wifi_pass;
  bool result = WiFi.softAP(ssid, pass);
  printWifiInfo(ssid, pass);
  return result;
}

bool startWiFiStaMode(String ssid, String pass) {
  bool result = WiFi.begin(ssid, pass);
  Serial.print("WiFi Connecting");
  bool wifi_connected = false;
  long connectingStartTime = millis();
  while (!wifi_connected && millis() - connectingStartTime < connect_timeout) {
    delay(500);
    Serial.print(".");
    wifi_connected = (WiFi.status() == WL_CONNECTED);
  }
  Serial.println();
  if (wifi_connected) {
    wifi_mode = "STA";
    printWifiInfo(ssid, pass);
    return true;
  }
  return false;
}

void printWifiInfo(String ssid, String pass) {
  Serial.println("WiFi network mode: " + wifi_mode);
  Serial.println("WiFi network name: " + ssid);
  Serial.println("WiFi network password: " + pass);
  Serial.println("Host IP Address: " + getWifiIP().toString());
}

IPAddress getWifiIP() {
  return (wifiSettings.wifi_start_sta == "true" ? WiFi.localIP() : WiFi.softAPIP());
}
