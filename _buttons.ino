/*
   Buttons
*/

void checkPressedOnStartSafeModeBtn() {
  if (digitalRead(RESET_PIN) == HIGH) {
    safeMode = true;
  }
}

void checkPressedSafeModeBtn() {
  if (digitalRead(RESET_PIN) == HIGH) {
    printLedControlIp(getWifiIP());
    digitalWrite(RESET_PIN, HIGH);
  }
}
