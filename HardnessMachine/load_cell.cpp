#include "load_cell.h"
#include <Arduino.h>

#define DOUT_PIN  29 // Define the data output pin
#define CLK_PIN   31 // Define the clock pin

HX711_ADC scale(DOUT_PIN, CLK_PIN); // Create an instance of the HX711 library
// 0.6 unit per 7.9153g

const int calc_eeprom_address = 0;
unsigned long t;

void loadCellCalibration() {
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Configuring load cell...");

  scale.begin();

  float calibration_value;
  calibration_value = 100;

  EEPROM.get(calc_eeprom_address, calibration_value);

  bool _tare = true;
  scale.start(2000, _tare);
  if (scale.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    scale.setCalFactor(calibration_value); // user set calibration value (float), initial value 1.0 may be used for this sketch
    Serial.println("Startup is complete");
  }
  scale.tareNoDelay();
}

float loadCellCalculate() {
  static bool newDataReady = 0;
  const int serialPrintInterval = 20;

  if (scale.update()) newDataReady = true;
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = scale.getData();
      Serial.print("Load cell reading: ");
      Serial.println(i);
      newDataReady = 0;
      t = millis();
      return i;
    }
  }
}