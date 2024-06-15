
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <Wire.h>      // this is needed for FT6206
#include <Adafruit_FT6206.h>

void menuSetup(int state);

void menuLoop(int state);

void tftSetup(int state);

void tftMenu(int state);