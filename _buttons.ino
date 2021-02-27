/*
   Buttons
*/

void checkPressedOnStartSafeModeBtn() {
  if (digitalRead(RESET_PIN) == LOW) {
    Serial.println("Start in safe mode");
    safeMode = true;
  }
}

void checkPressedSafeModeBtn() {
  int btnState = digitalRead(RESET_PIN);
  if (btnState == LOW) {
    printLedControlIp(getWifiIP());
    digitalWrite(RESET_PIN, HIGH);
  }
}
