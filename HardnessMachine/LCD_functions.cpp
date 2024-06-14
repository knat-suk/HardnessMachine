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

// LCD defs
LiquidCrystal lcd(rs, enable, d4, d5, d6, d7);

// Machine state
int machineOn = 1;
char *list_of_states[] = {"compress", "moving", "indent", "camera"};
unsigned long startMillis;
unsigned long currentMillis;
unsigned long buttonCoolDown;
unsigned long ledCoolDown;
int lastInput;//=digitalRead(44);
int buttonState = LOW;

byte zero[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte one[] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};
byte two[] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000
};
byte three[] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100
};
byte four[] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110
};
byte five[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

void lcdSetup(int machine_state) {
  Serial2.begin(115200);
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.createChar(0, zero);
  lcd.createChar(1, one);
  lcd.createChar(2, two);
  lcd.createChar(3, three);
  lcd.createChar(4, four);
  lcd.createChar(5, five);

  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  // Display initial information
  lcd.clear();
  lcd.print("Machine Status:");
  lcd.setCursor(0, 1);
  lcd.print("ON");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Machine Status:");
  lcd.setCursor(0, 1);
  // lcd.setCursor(8, 1);
  // lcd.print(list_of_states[machine_state]);
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

int prev_state = -1;
void updateLCD(long init_time, int machine_state) {
  currentMillis = millis();
  if (currentMillis-startMillis <= 2000) {
    return;
  }
  if (machine_state==4) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Complete");
  }
  else if (machine_state==5) {
    return;
  }
  // lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Machine Status:");
  // lcd.setCursor(0, 1);
  // lcd.print(machineOn ? "ON" : "OFF");
  // lcd.setCursor(8, 1);
  // lcd.print(list_of_states[machine_state]);
  lcd.print("   ");
  if (prev_state!=machine_state) {
    lcdProgressBar((machine_state+1)*100/5, 100, 1);
    prev_state=machine_state;
  }
  startMillis = currentMillis;
}

void lcdProgressBar(unsigned long count, unsigned long totalCount, int lineToPrintOn) {
  for (int i=count; i<count+20; i++) {
    double factor = totalCount/80.0;
    int percent = (i+1)/factor;
    int number = percent/5;
    int remainder = percent % 5;
    if (number > 0) {
      lcd.setCursor(number-1, lineToPrintOn);
      lcd.write(5);
    }
    lcd.setCursor(number, lineToPrintOn);
    lcd.write(remainder);

  }

}

void lcdDisplayForce(double result) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Machine Status:");
  lcd.setCursor(0, 1);
  lcd.print("RES");
  lcd.setCursor(8, 1);
  lcd.print(result, 4);
}

void updateLED() {
  digitalWrite(LED_PIN, machineOn ? HIGH : LOW);
}
