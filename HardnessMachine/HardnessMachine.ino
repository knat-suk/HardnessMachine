#include "LCD_functions.h"
#include "motor_functions.h"
// #include "motor_accel.h"
#include "load_cell.h"
// #include "camera_functions.h"
#include <Arduino.h>

// Machine states: 0 = move sample, 1 = compress sample and revert, 2 = move sample, 3 = indent sample, 4 = move sample. 5 = take picture
#define ON_OFF_BUTTON 39
const int sw = 42;
const int x = 38;
const int y = 40;
#define tx 1
#define rx 0

int machine_state = 0;
unsigned long init_time;
unsigned long curr_millis;

int state = 0; // 0 = go, 1 = user menu. 2 = settings, 3 = reset

void menu(int &state) {
  double joy_stick_delay = 200;
  int curr_input = millis();
  int prev_input = millis();
  int y_state = 0;
  while (state!=-1) {
    curr_input = millis();
    lcdMenu(state, speed);
    int x_read = analogRead(x);
    int y_read = analogRead(y);

    if (curr_input-prev_input > 150) {
      if (x_read > 768) {
        state = (state+1) % 3;
        prev_input = curr_input;
      }
      else if (x_read < 256) {
        state = (state-1) % 3;
        prev_input = curr_input;
      }
      switch (state) {
        case 0:
          if (sw==0) {
            switch (y_read) {
              case 0: //low torque
                speed = 1200;
                break;
              case 1: //medium torque
                speed = 1000;
                break;
              case 2: //high torque
                speed = 700;
                break;
            }
            setup();
            state = -1;
          }
          break;
        case 1: //settings
          if (y_read > 768) {
            y_state = (y_state+1) % 3;
            prev_input = curr_input;
          }
          else if (y_read < 256) {
            y_state = (y_state-1) % 3;
            prev_input = curr_input;
          }
        case 2: //reset
          resetMotors();
      }
    }

  }
}

void setup() {
  Serial.begin(115200); //57600
  Serial.println("Machine is on");
  pinMode(tx, OUTPUT);
  pinMode(rx, INPUT_PULLUP);

  pinMode(ON_OFF_BUTTON, INPUT_PULLUP);
  pinMode(12, OUTPUT); // camera
  pinMode(sw, INPUT);
  digitalWrite(sw, HIGH);
  digitalWrite(tx, LOW);

  // set up lcd
  lcdSetup(machine_state);

  //set up motor
  motorSetup();

  // set up load cell 
  loadCellCalibration();
  int signal = digitalRead(ON_OFF_BUTTON);
  while (signal == 0) {
    signal = digitalRead(ON_OFF_BUTTON);
  }


  init_time = millis();
}

void loop() {

  curr_millis = millis();

  menu(state);

  // lcd loop
  lcdLoop(init_time, machine_state);

  // motor loop. Load cell is used when compressing
  motorLoop(machine_state, curr_millis);

  if (machine_state==4) {  // cameraSetup(); // take picture
    Serial.println("State: 4");
    Serial.println("Run Python Script");
    digitalWrite(12, HIGH); // turn on camera to access image via esp32 access point

    Serial.write("On");
    lcdDisplayForce(result_force);

    // Serial.println(result_force); // send force
    machine_state+=1;
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