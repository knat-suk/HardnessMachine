
#include <AccelStepper.h>
#include <MultiStepper.h>

#include "motor_functions.h"

#include <Arduino.h>

// motor pins and steps
// const int step_1 = 22;
// const int dir_1 = 2;
const int step_1 = 6; // platform
const int dir_1 = 7;
const int step_2 = 4; // indentor
const int dir_2 = 5;

const int switch_up = 9;
const int switch_down = 8;

#define actuator1 2
#define actuator2 3
#define STEPS_PER_REV 200 // motor steps 64 for 28by
// 0.2 cm p rev, full length 34.0 cm 

// variables
unsigned long prevMillis;
long position[3] = {500, 5900, 1500}; // base 
double force[500] = {0}; 
int n = 10; // n is number of force measurements
double result_force;
int interval = 750;

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
  pinMode(switch_up, INPUT_PULLUP);
  pinMode(switch_down, INPUT_PULLUP);
  digitalWrite(step_1,LOW);
  digitalWrite(step_2,LOW);

  result_force = 0.0;
}

bool record_time = false;
void motorLoop(int &machine_state, unsigned long curr_millis ) {


  if (machine_state==1) { // compress
    Serial << "State: " << machine_state << '\n';
    Serial << "Compress" <<'\n';
    compress();
  }

  else if (machine_state==0 || 2 || 4) { // move
    Serial << "State: " << machine_state << '\n';
    Serial << "Move" <<'\n';

    movePlatform(machine_state);
  }

  else if (machine_state==3) { // indent
    Serial << "State: " << machine_state << '\n';
    Serial << "Indent" <<'\n';
    indent(machine_state);
    Serial.println("Finished indenting");
    Serial.println("Calculating force...");
    for (int i=0; i<n; i++) {
      result_force += abs(force[i]);//*7.9153/0.6;
      Serial.print(i);
      Serial.print(" = ");
      Serial.println(force[i]);
    }
    Serial.println(result_force);
    result_force /= n;
    Serial.println("Force (g) = ");
    Serial.println(result_force);
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

int i=0;

//actuator
void compress() {
  digitalWrite(actuator1, HIGH);
  digitalWrite(actuator2, LOW);
  delay(1650);
  digitalWrite(actuator1, LOW);
  digitalWrite(actuator2, HIGH);
  delay(1650);
  digitalWrite(actuator2, LOW);
  delay(1000);
}

void movePlatform(int& machine_state) {
  if (machine_state==4) {
    delay(10000);
    step(position[int((machine_state)/2)], HIGH, interval, step_1, dir_1);
    return;
  }
  step(position[int((machine_state)/2)], LOW, interval, step_1, dir_1);
}

void indent(int& machine_state) { // move sample platform
    step(700, LOW, interval, step_2, dir_2);
    Serial.println("Currently indenting");
    Serial.println("Measuring force");
    // while (i<n) { // record force every 5 milliseconds
    //   force[i] = loadCellCalculate();
    //   i+=1;
    // }
    step(700, HIGH, interval, step_2, dir_2);
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

void moveManual() {
  delay(200);
  while (digitalRead(42)!=0) { // while sw is not pressed again
    // int state_up = digitalRead(switch_up);
    // int state_down = digitalRead(switch_down);
    int state_up = analogRead(A0);
    int state_down = analogRead(A1);

    if (state_up > 700) {
      step(1,HIGH,interval,step_1,dir_1);
    }
    else if (state_up < 600) {
      step(1,LOW,interval,step_1,dir_1);
    }
    if (state_down > 700) {
      step(1,HIGH,interval,step_2,dir_2);
    }
    else if (state_down<600) {
      step(1,LOW,interval,step_2,dir_2);
    }
  }
}
void changeInterval(double num) {
  interval = num;
}

