#include <Stepper.h>
#include "load_cell.h"

void motorSetup();

void motorLoop(int &machine_state, unsigned long curr_millis);

void movePlatform(int &machine_state);

void compress();

void indent(int &machine_state);

void stopButton();

void returnToOriginalPosition();

void test();
