/*
   Buttons
*/

void checkPressedOnStartSafeModeBtn() {
  if (digitalRead(RESET_PIN) == HIGH) {
    Serial.println("Starting in SAFE mode");
    safeMode = true;
  }
}

void checkPressedSafeModeBtn() {
  if (digitalRead(RESET_PIN) == HIGH) {
    printLedControlIp(getWifiIP());
    digitalWrite(RESET_PIN, HIGH);
  }
}
