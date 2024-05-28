#include "LCD_functions.h"
#include "motor_functions.h"
// #include "motor_accel.h"
#include "load_cell.h"
#include "camera_functions.h"
#include <Arduino.h>

// Machine states: 0 = move sample, 1 = compress sample and revert, 2 = move sample, 3 = indent sample, 4 = move sample. 5 = take picture
#define ON_OFF_BUTTON 39
bool state = false; // false = on, true = off
int machine_state = 0;
unsigned long init_time;
unsigned long curr_millis;

void stop() {
  int input = digitalRead(ON_OFF_BUTTON);
  Serial.println(input);
  while (state) {
    if (input==0) {
      state = false;
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Machine is on");

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
  curr_millis = millis();

  // lcd loop
  lcdLoop(init_time, machine_state);

  // motor loop. Load cell is used when compressing
  motorLoop(machine_state, curr_millis);

  // cameraSetup(); // take picture
  if (machine_state==5) {
    Serial.println("State: 5");
    if (Serial.available() > 0) {
      Serial.println("Run Python Script");
    }
    // calculate avg_force

    // Run camera
    cameraExec(); // access image via esp32 access point
    Serial.println("Activate camera");
    // Serial.println(result_force); // send force
    machine_state+=1;
    // program will run indefinitely, as long as esp32 webserver stays on
  }
  delay(2);
}