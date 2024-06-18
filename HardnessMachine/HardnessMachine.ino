#include "LCD_functions.h"
#include "motor_functions.h"
#include "load_cell.h"
#include "tft_lcd.h"
#include <Arduino.h>

// Machine states: 0 = move sample, 1 = compress sample and revert, 2 = move sample, 3 = indent sample, 4 = move sample. 5 = take picture
#define ON_OFF_BUTTON 39
#define tx 1
#define rx 0

int machine_state = 0;
unsigned long curr_millis;

int state = 0; // 0 = go, 1 = user menu. 2 = settings, 3 = reset


void setup() {
  Serial.begin(57600); //57600
  Serial.println("Machine is on");

  // tftSetup(state);
  // menuSetup(state);

  // // set up lcd
  lcdSetup(machine_state);
  menuSetup(state);


  // //set up motor
  motorSetup();

  // // set up load cell 
  loadCellCalibration();
}

void loop() {

  curr_millis = millis();

  // tftMenu(state);

  // lcd loop
  lcdLoop(curr_millis, machine_state);

  // motor loop. Load cell is used when compressing
  motorLoop(machine_state, curr_millis);

  if (machine_state==4) {  // cameraSetup(); // take picture
    Serial.println("State: 4");
    Serial.println("Run Python Script");
    digitalWrite(12, HIGH); // turn on camera to access image via esp32 access point

    Serial.write("On");
    lcdDisplayForce(result_force);

    // Serial.println(result_force); // send force
    // machine_state+=1;
    // program will run indefinitely, as long as esp32 webserver stays on
  }
  else if (machine_state==5) {
    int on_signal = digitalRead(ON_OFF_BUTTON);
    if (on_signal==1) {
      machine_state==0;
    }
  }
  // Serial.print("machine= ");
  // Serial.println(machine_state);
  delay(2);
}