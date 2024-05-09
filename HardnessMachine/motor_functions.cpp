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
#define compButton 20
#define indentButton 18

// motor steps and instantiation
#define STEPS_PER_REV 64

// variables
unsigned long motorMillis;
unsigned long currMillis;

// be aware of swapping inputs
Stepper motor1(STEPS_PER_REV, 22, 2, 3, 4); // motor for platform
Stepper motor2(STEPS_PER_REV, 5, 6, 7, 8); // motor for compressing and indentation
Stepper motor3(STEPS_PER_REV, 9, 10, 11, 12); // motor for compressing and indentation
// Stepper motor2(STEPS_PER_REV, MOTOR2_STEP_PIN, MOTOR2_DIR_PIN);
// Stepper motor3(STEPS_PER_REV, MOTOR3_STEP_PIN, MOTOR3_DIR_PIN);


void motorSetup() {
  // Set motor speed and enable motor drivers if needed

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
    compress();
    motorMillis = millis();
    machine_state+=1;
  }

  else if (machine_state==0 || machine_state==2 || machine_state==4) {
    movePlatform();
    motorMillis = millis();
    machine_state+=1;
  }
  // if (currMillis-motorMillis > 3000) {
    // motor1.step(2048);
    // motor1.step(1000);
    // delay(10000);
    // motorMillis = currMillis;
  // }
}

//Motor 
bool compressFinish = false;
void compress() {
  // move platform to compress and indent sample
  if (!compressFinish) {
    //compress
    motor2.step(200);  
    // motor3.step(500);
  }
    motor2.step(-200);
    // motor3.step(-500);
}


bool moveFinish = false;
void movePlatform() {
  // move sample motor

  // while (!moveFinish) {
    motor1.step(200);
    stopButton();
  // }
}

void resetMotors() {
  // Serial.println(motor2.step_number);
}

void stopButton() {
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

