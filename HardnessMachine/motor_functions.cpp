#include <AccelStepper.h>
#include <MultiStepper.h>

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
#define STEPS_PER_REV 200 // motor steps 64 for 28by

// variables
unsigned long prevMillis;

// be aware of swapping inputs
MultiStepper steppers;

AccelStepper stepper1(AccelStepper::FULL4WIRE, 22, 2, 3, 4);
AccelStepper stepper2(AccelStepper::FULL4WIRE, 5, 6, 7, 8);
Stepper motor1(STEPS_PER_REV, 22, 2, 3, 4); // motor for platform
Stepper motor2(STEPS_PER_REV, 5, 6, 7, 8); // motor for indentation

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
  stepper1.setMaxSpeed(100);
  stepper2.setMaxSpeed(100);
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);

  pinMode(compButton, INPUT_PULLUP); //Pull up resistor, very important for cancelling noise
  pinMode(indentButton, INPUT_PULLUP);
  result_force = 0.0;
}
bool record_time = false
void motorLoop(int &machine_state, unsigned long curr_millis ) {
  // Motor operations
  if (machine_state==1 || machine_state==3) {
    Serial << "State: " << machine_state << '\n';
    compress();
    machine_state += 1;
    delay(1000);
  }

  else if (machine_state==0 || machine_state==2 || machine_state==4) {
    Serial << "State: " << machine_state << '\n';
    movePlatform(machine_state);
  }
  int input3 = digitalRead();
}

// Load Force 
bool compressFinish = false;
double force[500] = {0}; 
int i=0;

//actuator
bool actuator = false;
void compress() {
  actuator = true; // compress
  delay(1000);
  while (i<500) { // record force every 5 milliseconds
    force[i] = loadCellCalculate();
    i+=1; 
    delay(5);
  }
  actuator = false;
}

bool moveFinish = false;
long positions[3][2] = {{200,200},{400,400},{600,600}};
void movePlatform(int& machine_state) { // move sample platform
    steppers.moveTo(positions[(int) machine_state/2]);
    Serial.println((int) machine_state/2);
    steppers.runSpeedToPosition();
    stopButton();
    machine_state += 1;
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

void test() {
  actuator = true
}

