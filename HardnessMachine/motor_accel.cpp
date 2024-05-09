// #include "motor_accel.h"
// #include <Arduino.h>

// #define compButton 20
// #define indentButton 18
// #define imageButton 16

// AccelStepper motor1(22, 2, 3, 4);
// AccelStepper motor2(5, 6, 7, 8);

// void motorSetup() {
//   pinMode(compButton, INPUT_PULLUP);
//   pinMode(indentButton, INPUT_PULLUP);

//   motor1.setMaxSpeed(200);
//   motor1.setMaxSpeed(200);
//   motor1.moveTo(200);
//   motor2.setAcceleration(200);
//   motor2.setAcceleration(200);
//   motor2.moveTo(200);
// }

// void motorLoop(int& machine_state) {
//   if (machine_state==0 || machine_state ==2 || machine_state==4) {
//     motor1.run();
//   }

//   else if (machine_state==1 || machine_state==3) {
//     motor2.run();
//   }
//   stopButton(machine_state);
// }

// void stopButton(int& machine_state) {
//   int input1 = digitalRead(compButton);
//   int input2 = digitalRead(indentButton);
//   int input3 = digitalRead(imageButton);
//   if (input1==0 && machine_state==0) {
//     machine_state += 1;
//   }

//   if (input2==0 && machine_state==2) {
//     machine_state += 1;
//   }

//   if (input3==0 && machine_state==4) {
//     machine_state += 1;
//   }
// }

