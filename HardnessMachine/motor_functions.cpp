
#include <AccelStepper.h>
#include <MultiStepper.h>

#include "motor_functions.h"

#include <Arduino.h>

// motor pins and steps
#define M1_STEP_1 22
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
#define actuator1 33
#define actuator2 35
#define STEPS_PER_REV 200 // motor steps 64 for 28by

// variables
unsigned long prevMillis;
double force[500] = {0}; 
double result_force;

// be aware of swapping inputs
MultiStepper steppers;
MultiStepper stepperNo3;

AccelStepper stepper1(AccelStepper::FULL4WIRE, 22, 2, 3, 4);
AccelStepper stepper2(AccelStepper::FULL4WIRE, 5, 6, 7, 8);
AccelStepper stepper3(AccelStepper::FULL4WIRE, 24, 26, 28, 30);
Stepper motor1(STEPS_PER_REV, 22, 2, 3, 4); // motor for platform
Stepper motor2(STEPS_PER_REV, 5, 6, 7, 8); // motor for indentation
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

  pinMode(actuator1, OUTPUT);
  pinMode(actuator2, OUTPUT);

  pinMode(compButton, INPUT_PULLUP); //Pull up resistor, very important for cancelling noise
  pinMode(indentButton, INPUT_PULLUP);
  result_force = 0.0;
}
bool record_time = false;
void motorLoop(int &machine_state, unsigned long curr_millis ) {
  // Motor operations
  if (machine_state==0) {
    Serial << "State: " << machine_state << '\n';
    compress();
    machine_state += 1;
    delay(1000);
  }
  else if (machine_state==2) {
    Serial << "State: " << machine_state << '\n';
    indent(machine_state);
    Serial.println("finished indenting");
    machine_state += 1;
  }

  else if (machine_state==1 || machine_state==3) {
    Serial << "State: " << machine_state << '\n';
    movePlatform(machine_state);
    machine_state += 1;
  }
  else if (machine_state==4) {
    Serial.print("Measuring force...");
    for (int i=0; i<500; i++) {
      result_force += force[i];
    }
    result_force /= 500;
    machine_state+=1;
  }
  int input3 = digitalRead(31);
}

// Load Force 
bool compressFinish = false;
int i=0;

//actuator
bool actuator = false;
void compress() {
  actuator = true; // compress
  digitalWrite(actuator1, HIGH);

  delay(1000);
  digitalWrite(actuator1, LOW);
  digitalWrite(actuator2, HIGH);
  delay(1000);
  digitalWrite(actuator2, LOW);

  actuator = false;
}

bool moveFinish = false;
long positions[3][1] = {{300},{600},{900}}; 
void movePlatform(int& machine_state) {
  stepperNo3.moveTo(positions[(machine_state+1)/2]);
  stepperNo3.runSpeedToPosition();
  // stepper3.move(positions[0]);
  // stepper3.run();
}

// void movePlatform(int& machine_state) { // move platform using relays
//   for (int i=0; i<distance[machine_state]; i++) {
//     for (int j=0; j<4; j++) {
//       digitalWrite(24, step_sequence[j][0]);
//       digitalWrite(26, step_sequence[j][1]);
//       digitalWrite(28, step_sequence[j][2]);
//       digitalWrite(30, step_sequence[j][3]);
//     }
//   }
// }

long indentPos[2][2] = {{600,600},{1200,1200}};
void indent(int& machine_state) { // move sample platform
    stepper1.setCurrentPosition(1200);
    stepper2.setCurrentPosition(1200);
    steppers.moveTo(indentPos[0]);
    steppers.runSpeedToPosition();
    steppers.moveTo(indentPos[1]);
    steppers.runSpeedToPosition();
    Serial.println("currently indenting");
    // while (i<3000) { // record force every 5 milliseconds
    //   force[i] = loadCellCalculate();
    //   i+=1;
    // }
    Serial.println("finished measure");
    stopButton();
}

// int step_sequence[4][4] = {
//   {1,0,0,0},
//   {0,1,0,0},
//   {0,0,1,0},
//   {0,0,0,1}
// }

// void set_step(int w1, int w2, int w3, int w4) {
//   digitalWrite(22,w1);
//   digitalWrite(2,w2);
//   digitalWrite(3,w3);
//   digitalWrite(4,w4);

//   digitalWrite(5,w1);
//   digitalWrite(6,w2);
//   digitalWrite(7,w3);
//   digitalWrite(8,w4);
// }

// void step_motor(int steps, int delay_time) {
//   for (int i=0; i<steps; i++) {
//     for (int j=0; j<4; j++) {
//       set_step(step_sequence[j][0],step_sequence[j][1],step_sequence[j][2],step_sequence[j][3]);
//       delay(delay_time);
//     }
//   }
// }

// void indent(int& machine_state) { // for relay
//   step_motor(100,2);
// }

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
  actuator = true;
}

