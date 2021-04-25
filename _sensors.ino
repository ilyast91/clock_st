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

  aht10.begin();
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

float bmpReadHumidity() {
  return bmp280.readHumidity();
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

float aht10readTemp() {
  sensors_event_t humidity, temp;
  aht10.getEvent(&humidity, &temp);
  return temp.temperature;
}

float aht10readHumidity() {
  sensors_event_t humidity, temp;
  aht10.getEvent(&humidity, &temp);
  return humidity.relative_humidity;
}
