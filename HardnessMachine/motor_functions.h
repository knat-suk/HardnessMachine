#include <Stepper.h>
#include "load_cell.h"

extern double result_force;

extern int interval;

void motorSetup();

void motorLoop(int &machine_state, unsigned long curr_millis);

void movePlatform(int &machine_state);

void compress();

void indent(int &machine_state);

void step(int steps, int state, int delay, int step1, int dir1);

void stopButton();

void resetMotors();

void moveManual();

void test();

void changeInterval(double num);
