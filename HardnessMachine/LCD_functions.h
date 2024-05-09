#include <LiquidCrystal.h>

void lcdSetup(int machine_state);

// funciton to run in loop
void lcdLoop(long init_time, int machine_state);

void toggleMachineState();

void updateLCD(long init_time, int machine_state);

void updateLED();