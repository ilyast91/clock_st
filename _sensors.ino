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
  bme280.begin(0x76);
  bme280.setSampling(Adafruit_BME280::MODE_NORMAL,     /* Operating Mode. */
                     Adafruit_BME280::SAMPLING_X2,     /* Temp. oversampling */
                     Adafruit_BME280::SAMPLING_X16,    /* Pressure oversampling */
                     Adafruit_BME280::SAMPLING_X1,     /* Humidity oversampling */
                     Adafruit_BME280::FILTER_X16,      /* Filtering. */
                     Adafruit_BME280::STANDBY_MS_500); /* Standby time. */    
                     
  bmp280.begin(0x76);
  bmp280.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                     Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                     Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                     Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                     Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */    

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

float bmeReadTemperature() {
  return bme280.readTemperature();
}

float bmeReadPressure() {
  float p = bme280.readPressure();
  p = p * 0.00750062;
  return p;
}

float bmeReadAltitude() {
  return bme280.readAltitude(bpmLocalAltitude);
}

float bmeReadHumidity() {
  return bme280.readHumidity();
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
