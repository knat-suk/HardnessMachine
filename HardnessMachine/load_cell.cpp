#include "load_cell.h"
#include <Arduino.h>

#define DOUT_PIN  29 // Define the data output pin
#define CLK_PIN   31 // Define the clock pin

HX711 scale; // Create an instance of the HX711 library

void loadCellCalibration() {
  scale.begin(DOUT_PIN, CLK_PIN); // Initialize the scale
  scale.tare();

  //calibrate
  // Serial.println("calibrate scale with 100 g. Press any key when done.");
  // while(!Serial.available());
  // while(Serial.available()) Serial.read();
  scale.calibrate_scale(1, 10);
  // Serial.println("success!");

  // Serial.print("Units: ");
  // Serial.println(scale.get_units(10));
  delay(1000);
}

float loadCellCalculate() {
  // float load = scale.read_average(10);
  float load = scale.get_units(10); // Read the load cell value in raw units
  // Print the load cell value to the serial monitor
  // Serial.print("load: ");
  Serial.println(abs(load));
  return load;
}