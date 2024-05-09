
#include <WebSockets.h>

#include "LCD_functions.h"
#include "motor_functions.h"
// #include "motor_accel.h"
#include "load_cell.h"
#include <Arduino.h>

// Machine states: 0 = move sample, 1 = compress sample and revert, 2 = move sample, 3 = indent sample, 4 = move sample. 5 = take picture
bool on_off = false;
int machine_state = 0;
unsigned long init_time;

void setup() {
  Serial.begin(115200);

  // set up lcd
  lcdSetup(machine_state);

  //set up motor
  motorSetup();

  // set up load cell
  loadCellCalibration();

  init_time = millis();
}

void loop() {
  // if (on_off = false) {
  //   return;
  // }

  // lcd loop
  loadCellCalculate();
  lcdLoop(init_time, machine_state);

  // motor loop
  motorLoop(machine_state);

  // cameraSetup(); // take picture
  if (machine_state==5) {
    if (Serial.available() > 0) {
      Serial.println("Run Python Script");
    }

    // Run camera
    // cameraExec();
    machine_state += 1;
  }
  delay(2);
}