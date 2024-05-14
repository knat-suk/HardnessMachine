#include "motor_functions.h"
#include <Arduino.h>

// motor pins and steps
#define M1_STEP_1 1
#define M1_STEP_2 2
#define M1_DIR_1 3
#define M1_DIR_2 4
#define M2_STEP_1 5
#define M2_STEP_2 6
#define M2_DIR_1 7
#define M2_DIR_2 8
#define M3_STEP_1 9
#define M3_STEP_2 10
#define M3_DIR_1 11
#define M3_DIR_2 12
#define compButton 23
#define indentButton 25
#define STEPS_PER_REV 64 // motor steps

// variables
unsigned long currMillis;
unsigned long prevMillis;

// be aware of swapping inputs
Stepper motor1(STEPS_PER_REV, 22, 2, 3, 4); // motor for platform
Stepper motor2(STEPS_PER_REV, 5, 6, 7, 8); // motor for indentation
Stepper motor3(STEPS_PER_REV, 9, 10, 11, 12); // motor for compressing and indentation
// Stepper motor2(STEPS_PER_REV, MOTOR2_STEP_PIN, MOTOR2_DIR_PIN);
// Stepper motor3(STEPS_PER_REV, MOTOR3_STEP_PIN, MOTOR3_DIR_PIN);

// FUNCTIONS
void motorSetup() {
  motorMillis = millis();
  currMillis = motorMillis;
  motor1.setSpeed(100);
  motor2.setSpeed(100);
  motor3.setSpeed(100);
  pinMode(compButton, INPUT_PULLUP);
  pinMode(indentButton, INPUT_PULLUP);
}

void motorLoop(int &machine_state) {
  currMillis = millis();

  // Motor operations
  if (machine_state==1 || machine_state==3) {
    prevMillis = currMillis;
    compress();
    machine_state+=1;
  }

  else if (machine_state==0 || machine_state==2 || machine_state==4) {
    prevMillis = currMillis;
    movePlatform();
    machine_state+=1;
  }

}

// Load Force 
bool compressFinish = false;
double force[500] = {0}; 
double result_force = 0.0;
int i=0;

//actuator
bool actuator = false;
void compress() {
  while (!compressFinish) {
    //compress
    actuator = true;
    force[i] = loadCellCalculate();
    i+=1;
    if (i==500) {i=0;} 
    stopButton();
    if (currMillis-prevMillis > 30000) {compressFinish = true}; // if greater than 30s, skip
  }
  actuator = false;
}

// void compress() {
//   // move platform to compress and indent sample
//   while (!compressFinish) {
//     //compress
//     motor2.step(200);  
//     force[i] = loadCellCalculate();
//     i+=1;
//     if (i==500) {i=0;} 
//     stopButton();
//   }

//   motor2.step(-200);
//   for (int i=0; i<500; i++) {
//     result_force += force[i];
//   }
//   result_force /= 500;
// }

bool moveFinish = false;
void movePlatform() { // move sample platform
  while (!moveFinish) {
    motor1.step(200);
    stopButton();
    if (currMillis-prevMillis > 30000) {compressFinish = true}; // if greater than 30s, skip
  }
  motor1.step(-200);
}

void resetMotors() { // for resetting all motors
  // Serial.println(motor2.step_number); 
}

void stopButton() { // for signalling when to stop compression/indentation
  int input1 = digitalRead(compButton);
  int input2 = digitalRead(indentButton);

  // button cool down code when pressed, toggles lcd
    if (input1 == 0) {
      moveFinish = true;
    }

    if (input2 == 0) {
      compressFinish = true;
    }
}

