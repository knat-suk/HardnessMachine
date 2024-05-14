#include "LCD_functions.h"
#include "motor_functions.h"
// #include "motor_accel.h"
#include "load_cell.h"
#include <Arduino.h>

// Machine states: 0 = move sample, 1 = compress sample and revert, 2 = move sample, 3 = indent sample, 4 = move sample. 5 = take picture
#define ON_OFF_BUTTON 39
bool off = false;
int machine_state = 0;
unsigned long init_time;

void stop() {
  while (off) {
    int input = digitalRead(ON_OFF_BUTTON);
    if (input==0) {
      off = false;
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ON_OFF_BUTTON, INPUT_PULLUP);

  // set up lcd
  lcdSetup(machine_state);

  //set up motor
  motorSetup();

  // set up load cell 
  loadCellCalibration();

  init_time = millis();
}

void loop() {
  stop();

  // lcd loop
  lcdLoop(init_time, machine_state);

  // motor loop. Load cell is used when compressing
  motorLoop(machine_state);

  // cameraSetup(); // take picture
  if (machine_state==5) {
    if (Serial.available() > 0) {
      Serial.println("Run Python Script");
    }
    // Run camera
    cameraExec(); // access image via esp32 access point
    // program will run indefinitely, as long as esp32 webserver stays on
  }
  delay(2);
}