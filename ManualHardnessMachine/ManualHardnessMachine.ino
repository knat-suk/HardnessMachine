//Manual
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Stepper.h>


#include <Arduino.h>

#define ON_Button 39

#define LED_PIN 13
#define rs 43
#define enable 45
#define d4 47
#define d5 49
#define d6 51
#define d7 53
#define button 39
#define compButton 23
#define indentBUtton 25

#define DOUT_PIN  29 // Define the data output pin
#define CLK_PIN   31 // Define the clock pin

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
#define button1 2
#define button2 3

MultiStepper steppers;
MultiStepper stepperNo3;

AccelStepper stepper1(AccelStepper::FULL4WIRE, 22, 2, 3, 4);
AccelStepper stepper2(AccelStepper::FULL4WIRE, 5, 6, 7, 8);
AccelStepper stepper3(AccelStepper::FULL4WIRE, 24, 26, 28, 30);
Stepper motor1(STEPS_PER_REV, 22, 2, 3, 4); // motor for platform
Stepper motor2(STEPS_PER_REV, 5, 6, 7, 8); // motor for indentation

void setup() {
  // put your setup code here, to run once:
  stepper1.setMaxSpeed(100);
  stepper2.setMaxSpeed(100);
  stepper3.setMaxSpeed(100);
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  stepperNo3.addStepper(stepper3);

  pinMode(actuator1, OUTPUT);
  pinMode(actuator2, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  String input = Serial.readString();
  int read = digitalRead(button1);
  int read2 = digitalRead(button2);
  if (read == 0) {
    digitalWrite(actuator1, HIGH);
    delay(2000);
    digitalWrite(actuator1, LOW);
  }
  else if (read2==0) {
    digitalWrite(actuator2, HIGH);
    delay(2000);
    digitalWrite(actuator2, LOW);
  }

}
