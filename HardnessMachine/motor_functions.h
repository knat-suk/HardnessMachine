#include <Stepper.h>
#include "load_cell.h"


void motorSetup();

void motorLoop(int &machine_state, unsigned long curr_millis);

void compress();

void stopButton();

void movePlatform(int &machine_state);

void returnToOriginalPosition();
