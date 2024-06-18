#include <LiquidCrystal.h>

void lcdSetup(int machine_state);

// funciton to run in loop
void lcdLoop(long init_time, int machine_state);

void lcdProgressBar(unsigned long count, unsigned long totalCount, int lineToPrintOn);

void lcdDisplayForce(double result_force);

void toggleMachineState();

void updateLCD(long init_time, int machine_state);

void lcdMenu(int state, int speed);

void lcdResetMotors();

void updateLED();