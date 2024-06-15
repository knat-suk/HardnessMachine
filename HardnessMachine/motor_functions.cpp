
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

// const int step_1 = 22;
// const int dir_1 = 2;
const int step_1 = 8; // platform
const int dir_1 = 9;
const int step_2 = 6; // indentor
const int dir_2 = 7;

#define actuator1 33
#define actuator2 35
#define STEPS_PER_REV 200 // motor steps 64 for 28by
// 0.2 cm p rev, full length 34.0 cm 

// variables
unsigned long prevMillis;
long position[3] = {900, 5500, 6450}; 
double force[500] = {0}; 
int n = 10; // n is number of force measurements
double result_force;
int interval = 2000;

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
  pinMode(step_1, OUTPUT);
  pinMode(step_2, OUTPUT);
  pinMode(dir_1, OUTPUT);
  pinMode(dir_2, OUTPUT);
  pinMode(actuator1, OUTPUT);
  pinMode(actuator2, OUTPUT);
  result_force = 0.0;
  step(1000, HIGH, 700, step_2, dir_2);
  step(1000, LOW, 700, step_2, dir_2);
  step(1000, HIGH, 700, step_1, dir_1);
  step(1000, LOW, 700, step_1, dir_1);
}
bool record_time = false;
void motorLoop(int &machine_state, unsigned long curr_millis ) {
  // Motor operations
  if (machine_state==1) { // compress
    Serial << "State: " << machine_state << '\n';
    Serial << "Compress" <<'\n';
    compress();
  }

  else if (machine_state==0 || 2 || 4) { // compress
    Serial << "State: " << machine_state << '\n';
    Serial << "Move" <<'\n';

    movePlatform(machine_state);
  }

  else if (machine_state==3) {
    Serial << "State: " << machine_state << '\n';
    Serial << "Indent" <<'\n';
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
  else if (machine_state==5) {
    Serial.print("force is = ");
    Serial.println(result_force);
  }
  else if (machine_state==6) {
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
  delay(1650);
  digitalWrite(actuator1, LOW);
  digitalWrite(actuator2, HIGH);
  delay(1650);
  digitalWrite(actuator2, LOW);
  delay(1000);

  actuator = false;
}

void movePlatform(int& machine_state) {
  if (machine_state==4) {
    step(position[int((machine_state)/2)], HIGH, interval, step_1, dir_1);
    return;
  }
  step(position[int((machine_state)/2)], LOW, interval, step_1, dir_1);
}

void indent(int& machine_state) { // move sample platform
    step(1000, HIGH, interval, step_2, dir_2);
    Serial.println("Currently indenting");
    Serial.println("Measuring force");
    // while (i<n) { // record force every 5 milliseconds
    //   force[i] = loadCellCalculate();
    //   i+=1;
    // }
    step(1000, LOW, interval, step_2, dir_2);
    Serial.println("Finished measuring");
}

void step(int steps, int state, int delay_time, int step_pin, int dir_pin) {
  digitalWrite(dir_pin,state); //Changes the rotations direction
  for(int x = 0; x < steps; x++) {
    digitalWrite(step_pin,HIGH);
    delayMicroseconds(delay_time);
    digitalWrite(step_pin,LOW);
    delayMicroseconds(delay_time);
  }
  delay(1000);
}

void resetMotors() { // for resetting all motors
  step(1000, LOW, interval, step_1, dir_1);
  step(1200, LOW, interval, step_2, dir_2);
  //
  digitalWrite(actuator1, LOW);
  digitalWrite(actuator2, HIGH);
  delay(4000);
  digitalWrite(actuator2, LOW);
}
void changeInterval(double num) {
  interval = num;
}

