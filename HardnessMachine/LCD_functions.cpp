#include "LCD_functions.h"
#include <Arduino.h>

//LCD pins
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

// LCD defs
LiquidCrystal lcd(rs, enable, d4, d5, d6, d7);

// Machine state
int machineOn = 1;
char *list_of_states[] = {"moving", "compress", "moving", "indent", "moving", "camera"};
unsigned long startMillis;
unsigned long currentMillis;
unsigned long buttonCoolDown;
unsigned long ledCoolDown;
int lastInput;//=digitalRead(44);
int buttonState = LOW;

void lcdSetup(int machine_state) {

  // Initialize LCD
  lcd.begin(16, 2);

  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  // Display initial information
  lcd.clear();
  lcd.print("Machine Status:");
  lcd.setCursor(0, 1);
  lcd.print("ON");
  lcd.setCursor(8, 1);
  lcd.print(list_of_states[machine_state]);
  startMillis = millis();
  ledCoolDown = startMillis;
}

// funciton to run in loop
void lcdLoop(long init_time, int machine_state) {

  // time keeper
  currentMillis = millis();
  
  // button
  int input = digitalRead(button);
  
  // button cool down code when pressed, toggles lcd
  if (currentMillis-buttonCoolDown >= 1000) {
    if (input == 0) {
      toggleMachineState();
      buttonCoolDown = millis();
    }
  }

  // Update LCD display based on machine state
  updateLCD(init_time, machine_state);

  // Update LED state based on machine state
  if (currentMillis-ledCoolDown >= 200) {
      updateLED();
      ledCoolDown = currentMillis;
  }
}

void toggleMachineState() {
  machineOn = !machineOn;
}

void updateLCD(long init_time, int machine_state) {
  currentMillis = millis();
  if (currentMillis-startMillis <= 2000) {
    return;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Machine Status:");
  lcd.setCursor(0, 1);
  lcd.print(machineOn ? "ON" : "OFF");
  lcd.setCursor(8, 1);
  lcd.print(list_of_states[machine_state]);
  startMillis = currentMillis;
}

void updateLED() {
  digitalWrite(LED_PIN, machineOn ? HIGH : LOW);
}
