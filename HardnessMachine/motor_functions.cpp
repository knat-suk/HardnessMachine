
#include <AccelStepper.h>
#include <MultiStepper.h>

#include "motor_functions.h"

#include <Arduino.h>

// motor pins and steps
// #define M1_STEP_1 22
// #define M1_STEP_2 2
// #define M1_DIR_1 3
// #define M1_DIR_2 4
// #define M2_STEP_1 5
// #define M2_STEP_2 6
// #define M2_DIR_1 7
// #define M2_DIR_2 8
// #define M3_STEP_1 9
// #define M3_STEP_2 10
// #define M3_DIR_1 11
// #define M3_DIR_2 12
#define compButton 23
#define indentButton 25

const int step_1 = 22;
const int dir_1 = 2;
const int step_2 = 3;
const int dir_2 = 4;
const int step_3 = 5;
const int dir_3 = 6;

#define actuator1 33
#define actuator2 35
#define STEPS_PER_REV 200 // motor steps 64 for 28by
// 0.2 cm p rev, full length 34.0 cm 

// variables
unsigned long prevMillis;
double force[500] = {0}; 
int n = 10; // n is number of force measurements
double result_force;
int speed = 2000;

// be aware of swapping inputs
MultiStepper steppers;
MultiStepper stepperNo3;

// AccelStepper stepper1(AccelStepper::FULL4WIRE, 22, 2, 3, 4);
// AccelStepper stepper2(AccelStepper::FULL4WIRE, 5, 6, 7, 8);
// AccelStepper stepper3(AccelStepper::FULL4WIRE, 24, 26, 28, 30);

AccelStepper stepper1(AccelStepper::DRIVER, 22, 2);
AccelStepper stepper2(AccelStepper::DRIVER, 22, 2);
AccelStepper stepper3(AccelStepper::DRIVER, 3, 4);
// Stepper motor1(STEPS_PER_REV, 22, 2, 3, 4); // motor for platform
// Stepper motor2(STEPS_PER_REV, 5, 6, 7, 8); // motor for indentation
// Stepper motor3(STEPS_PER_REV, 24, 26, 28, 30);

template <typename T>
Print& operator<<(Print& printer, T value)
{
    printer.print(value);
    return printer;
}
// FUNCTIONS
void motorSetup() {
  // motor1.setSpeed(100);
  // motor2.setSpeed(100);
  // motor3.setSpeed(100);
  stepper1.setMaxSpeed(100);
  stepper2.setMaxSpeed(100);
  stepper3.setMaxSpeed(100);
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  stepperNo3.addStepper(stepper3);

  pinMode(step_1, OUTPUT);
  pinMode(step_2, OUTPUT);
  pinMode(step_3, OUTPUT);
  pinMode(actuator1, OUTPUT);
  pinMode(actuator2, OUTPUT);

  pinMode(compButton, INPUT_PULLUP); //Pull up resistor, very important for cancelling noise
  pinMode(indentButton, INPUT_PULLUP);
  digitalWrite(actuator1, LOW);
  digitalWrite(actuator2, HIGH);
  delay(500);
  digitalWrite(actuator2, LOW);
  result_force = 0.0;
}
bool record_time = false;
void motorLoop(int &machine_state, unsigned long curr_millis ) {
  // Motor operations
  if (machine_state==0) {
    Serial << "State: " << machine_state << '\n';
    compress();
    delay(1000);
  }

  else if (machine_state==1 || machine_state==3 || 5) {
    Serial << "State: " << machine_state << '\n';
    movePlatform(machine_state);
  }

  else if (machine_state==2) {
    Serial << "State: " << machine_state << '\n';
    indent(machine_state);
    Serial.println("Finished indenting");
    Serial.println("Calculating force...");

    // for (int i=0; i<n; i++) {
    //   result_force += abs(force[i]);//*7.9153/0.6;
    //   Serial.println(force[i]);
    // }
    // Serial.println(result_force);
    // result_force /= n;
    // Serial.println("Force (g) = ");
    // Serial.println(result_force);
  }
  else if (machine_state==4) {
    Serial.print("force is = ");
    Serial.println(result_force);
  }
  else if (machine_state==5) {
    return;
  }
  machine_state += 1;
}

// Load Force 
bool compressFinish = false;
bool moveFinish = false;
int i=0;

//actuator
bool actuator = false;
void compress() {
  actuator = true; // compress
  digitalWrite(actuator1, HIGH);
  digitalWrite(actuator2, LOW);
  delay(4000);
  digitalWrite(actuator1, LOW);
  digitalWrite(actuator2, HIGH);
  delay(4000);
  digitalWrite(actuator2, LOW);

  actuator = false;
}

long position[3] = {3600, 1200, 4800}; 
void movePlatform(int& machine_state) {
  // stepper3.moveTo(1700);
  // stepper3.runSpeedToPosition();
  if (machine_state==5) {
    step(position[int((machine_state-1)/2)], HIGH, speed, step_3, dir_3, step_3, dir_3);
    return;
  }
  step(position[int((machine_state-1)/2)], LOW, speed, step_3, dir_3, step_3, dir_3);
}

// long indentPos[2][2] = {{600,600},{1200,1200}};
void indent(int& machine_state) { // move sample platform
    // stepper1.setCurrentPosition(1200);
    // stepper2.setCurrentPosition(1200);
    // steppers.moveTo(indentPos[0]);
    // steppers.runSpeedToPosition();
    step(1000, HIGH, speed, step_1, dir_1, step_2, dir_2);
    Serial.println("Currently indenting");
    Serial.println("Measuring force");
    while (i<n) { // record force every 5 milliseconds
      force[i] = loadCellCalculate();
      i+=1;
    }
    // steppers.moveTo(indentPos[1]);
    // steppers.runSpeedToPosition();
    step(1000, LOW, speed, step_1, dir_1, step_2, dir_2);
    Serial.println("Finished measuring");
}

void step(int steps, int state, int delay_time, int step_1, int dir_1, int step_2, int dir_2) {
  digitalWrite(dir_1,state); //Changes the rotations direction
  digitalWrite(dir_2,state); //Changes the rotations direction

  for(int x = 0; x < steps; x++) {
    digitalWrite(step_1,HIGH);
    digitalWrite(step_2,HIGH); 
    delayMicroseconds(delay_time);
    digitalWrite(step_1,LOW);
    digitalWrite(step_2,LOW); 
    delayMicroseconds(delay_time);
  }
  delay(1000);
}

void resetMotors() { // for resetting all motors
  long pos[2] = {0,0};
  steppers.moveTo(pos);
}

void stopButton() { // for signalling when to stop compression/indentation
  int input1 = digitalRead(compButton);
  int input2 = digitalRead(indentButton);

  // button cool down code when pressed, toggles lcd
  if (input1 == 0) {
    Serial.println("stop platform");
    moveFinish = true;
  }

  if (input2 == 0) {
    Serial.println("stop compress");
    compressFinish = true;
  }
}

