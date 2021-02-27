/*
   Sensors
*/

float dallasTempStore[10];
long dallasUpdateTime = 0;
long dallasUpdateInterval = 3000;

float bpmLocalAltitude = 1013.25;

void startSensors() {
  Serial.print("Sensors starting: ");
  sensors.begin();
  sensors.requestTemperatures();
  for (uint8_t i = 0; i < sensors.getDS18Count(); i++) {
    dallasTempStore[i] = sensors.getTempCByIndex(i);
  }

  float altitude = sensorsSettings.bmp_local_altitude.toFloat();
  if (altitude > 0) bpmLocalAltitude = altitude;  
  bmp280.begin(0x76);
  /* Default settings from datasheet. */
  bmp280.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                     Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                     Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                     Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                     Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  Serial.println("ok");
}

float bmpReadTemperature() {
  return bmp280.readTemperature();
}

float bmpReadPressure() {
  float p = bmp280.readPressure();
  p = p * 0.00750062;
  return p;
}

float bmpReadAltitude() {
  return bmp280.readAltitude(bpmLocalAltitude);
}

float dallasReadTemp(int index) {
  if (dallasUpdateTime == 0) dallasUpdateTime = millis();
  if (millis() - dallasUpdateTime > dallasUpdateInterval) {
    dallasTempStore[index] = sensors.getTempCByIndex(index);
    sensors.setWaitForConversion(false);  // makes it async
    sensors.requestTemperatures();
    sensors.setWaitForConversion(true);
    dallasUpdateTime = millis();
  }
  return dallasTempStore[index];
}
