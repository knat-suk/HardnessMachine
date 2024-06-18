#include <LiquidCrystal.h>
#include <HX711_ADC.h>
#include <EEPROM.h>

const int actuator1 = 2;

const int actuator2 = 3;

const int stepPinIndenter = 4;

const int dirPinIndenter = 5;

const int stepPinBase = 6;

const int dirPinBase = 7;

int interval = 750;

const int movementBeforeCompaction = 470;

const int movementAfterCompaction = 5850;

const int indenterDownMovement = 900;

const int indenterUpMovement = 900;

const int movementAfterIndenter = 2000;

//const int switchDown = 8;

//const int switchUp = 9;

const int b = 42;

const int rs = 43;
const int enable = 45;
const int d4 = 47;
const int d5 = 49;
const int d6 = 51;
const int d7 = 53;
LiquidCrystal lcd(rs, enable, d4, d5, d6, d7);

int machine_state = 0;
double result_force;
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

void startLCD() {
  lcd.begin(16, 2);
  lcd.createChar(0, zero);
  lcd.createChar(1, one);
  lcd.createChar(2, two);
  lcd.createChar(3, three);
  lcd.createChar(4, four);
  lcd.createChar(5, five);
  lcd.clear();
  lcd.print("Machine Status:");
  lcd.setCursor(0, 1);
  lcd.print("ON");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Machine Status:");
  lcd.setCursor(0, 1);
}

void start() {
  pinMode(b, INPUT);
  digitalWrite(b, HIGH);
  while (digitalRead(b)!=0) {
    Serial.println("off");
  }
}

void setup() {
  Serial.begin(57600);
  loadCellCalibration();
  startLCD();
  start();
  //for LEDs

  pinMode(actuator1, OUTPUT);

  pinMode(actuator2, OUTPUT);

  //for switches

  //pinMode(switchDown, INPUT);

  //pinMode(switchUp, INPUT);

  //for stepper motors indenter

  pinMode(stepPinIndenter, OUTPUT);

  pinMode(dirPinIndenter, OUTPUT);

  //for stepper motors base

  pinMode(stepPinBase, OUTPUT);

  pinMode(dirPinBase, OUTPUT);

 

}

 
int state = 0;
void loop() {
  menuLoop(state);
  lcd.clear();

  updateLCD(machine_state,0);

  digitalWrite(actuator1, HIGH);

  delay(2500);

  digitalWrite(actuator1, LOW);

  machine_state+=1;

  updateLCD(machine_state,0);

  //movement of base ot compactor

  //high is in the front

  digitalWrite(dirPinBase, HIGH);

  for(int x = 0; x < movementBeforeCompaction; x++) {

    digitalWrite(stepPinBase, HIGH);

    delayMicroseconds(interval);

    digitalWrite(stepPinBase, LOW);

    delayMicroseconds(interval);

  }

  delay(1000);
  machine_state+=1;

  updateLCD(machine_state,0);
 

//actuator movement

  digitalWrite(actuator2, HIGH);

  delay(1650);

  digitalWrite(actuator2, LOW);

  digitalWrite(actuator1, HIGH);

  delay(1650);

  digitalWrite(actuator1, LOW);

  machine_state+=1;

  updateLCD(machine_state,0);

//base moves to indenter

  digitalWrite(dirPinBase, HIGH);

  for (int x = 0; x < movementAfterCompaction; x++) {

    digitalWrite(stepPinBase, HIGH);

    delayMicroseconds(interval);

    digitalWrite(stepPinBase, LOW);

    delayMicroseconds(interval);

  }

  delay(1000);
  machine_state+=1;

  updateLCD(machine_state,0);

//indenter goes down

  digitalWrite(dirPinIndenter, LOW); //high goes up

  for(int x = 0; x < indenterDownMovement; x++) {

    digitalWrite(stepPinIndenter, HIGH);

    delayMicroseconds(interval);

    digitalWrite(stepPinIndenter, LOW);

    delayMicroseconds(interval);

  }

  delay(1000);
  int i=0;
  double force[10] = {0};
  while (i<10) { // record force every 5 milliseconds
    force[i] = loadCellCalculate();
    result_force += force[i];
    i+=1;
    delay(20);
  }
  result_force = result_force/10;
  machine_state+=1;

  updateLCD(machine_state,0);
 

//indenter goes up

  digitalWrite(dirPinIndenter, HIGH); //high goes up

 

  for(int x = 0; x < indenterUpMovement; x++) {

    digitalWrite(stepPinIndenter, HIGH);

    delayMicroseconds(interval);

    digitalWrite(stepPinIndenter, LOW);

    delayMicroseconds(interval);

  }

  delay(1000);

  machine_state+=1;

  updateLCD(machine_state,0);

//base goes to camera

  digitalWrite(dirPinBase, HIGH);

  for(int x = 0; x < movementAfterIndenter; x++) {

    digitalWrite(stepPinBase, HIGH);

    delayMicroseconds(interval);

    digitalWrite(stepPinBase, LOW);

    delayMicroseconds(interval);

  }

  delay(2500);

 

  digitalWrite(dirPinBase, LOW);

  for(int x = 0; x < (movementAfterIndenter + movementAfterCompaction + movementBeforeCompaction); x++) {

    digitalWrite(stepPinBase, HIGH);

    delayMicroseconds(interval);

    digitalWrite(stepPinBase, LOW);

    delayMicroseconds(interval);

  }
  lcd.clear();
  lcd.print("Machine Status:");
  lcd.setCursor(0, 1);
  lcd.print(result_force);
  delay(2000);
  lcd.clear();
  state = 0;

}









#define DOUT_PIN  29 // Define the data output pin
#define CLK_PIN   31 // Define the clock pin

HX711_ADC scale(DOUT_PIN, CLK_PIN); // Create an instance of the HX711 library
// 0.6 unit per 7.9153g

const int calc_eeprom_address = 0;
unsigned long t;

void loadCellCalibration() {
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Configuring load cell...");

  scale.begin();

  float calibration_value;
  calibration_value = 100;

  EEPROM.get(calc_eeprom_address, calibration_value);

  bool _tare = true;
  scale.start(2000, _tare);
  if (scale.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    scale.setCalFactor(calibration_value); // user set calibration value (float), initial value 1.0 may be used for this sketch
    Serial.println("Startup is complete");
  }
  scale.tareNoDelay();
}

float loadCellCalculate() {
  static bool newDataReady = 0;
  const int serialPrintInterval = 20;

  if (scale.update()) newDataReady = true;
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = scale.getData();
      Serial.print("Load cell reading: ");
      Serial.println(i);
      newDataReady = 0;
      t = millis();
      return i;
    }
  }
}

bool set = 1;
void updateLCD(long init_time, int machine_state) {
  if (set==1) {lcd.clear(); set=false;}
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
  lcd.print("   ");
  // if (prev_state!=machine_state) {
  lcdProgressBar((machine_state)*100/5, 100, 1);
    // prev_state=machine_state;
  // }
  // startMillis = currentMillis;
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

String menus[3] = {"Run", "Setting", "Reset"};
void lcdMenu(int state, int speed) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(menus[state]+ " <==");
  lcd.setCursor(0, 1);
  lcd.print(menus[(state+1)%3]);
  if (state==1) {
    lcd.setCursor(12, 0);
    lcd.print(speed);
  }
  else if (state == 0) {
    lcd.setCursor(12, 1);
    lcd.print(speed);
  }
}

double curr_input = 0;
int x = A1;
int y = A0;
int sw = 42;
int y_state = 0;
double prev_input = 0;


void menuLoop(int state) {
  lcdMenu(state, interval);

  while (state != -1) {
    delay(200);
    curr_input = millis();
    int x_read = analogRead(x);
    int y_read = analogRead(y);
    int sw_read = digitalRead(sw);
    Serial.print("x = ");
    Serial.println(x_read);
    Serial.print("y = ");
    Serial.println(y_read);
    Serial.print("sw = ");
    Serial.println(sw_read);
    Serial.print("state = ");
    Serial.println(state);

    Serial.print(state);
    if (curr_input - prev_input > 200) {
      if (x_read > 768) {
        state = (state + 1) % 3;
        prev_input = curr_input;
        lcdMenu(state,interval);
        Serial.print("xup ");
        Serial.println(state);
      } 
      else if (x_read < 256) {
        state = (state - 1) % 3;
        if (state < 0) { state += 3; }
        prev_input = curr_input;
        lcdMenu(state,interval);
        Serial.print("xdown= ");
        Serial.println(state);
      }
      switch (state) {
        case -1:
          Serial.println("run");
        case 0:
          if (sw_read == 0) {
            state = -1;
          }
          break;
        case 1:  //settings
          if (y_read < 25) {
            y_state = (y_state - 1) % 3;
            if (y_state<0) {y_state=1;}
            prev_input = curr_input;
            interval += 100;
            if (interval >2000) {interval = 2000;}
            lcdMenu(state,interval);
          }
          else if (y_read < 127) {
            y_state = (y_state - 1) % 3;
            if (y_state<0) {y_state=1;}
            prev_input = curr_input;
            interval += 10;
            if (interval >2000) {interval = 2000;}
            lcdMenu(state,interval);
          }
          else if (y_read < 500) {
            y_state = (y_state - 1) % 3;
            if (y_state<0) {y_state=1;}
            prev_input = curr_input;
            interval += 1;
            if (interval >2000) {interval = 200;}
            lcdMenu(state,interval);
          } 
          else if (y_read > 1000) {
            y_state = (y_state + 1) % 3;
            prev_input = curr_input;
            interval -= 100;
            if (interval <1) {interval = 1;}
            lcdMenu(state,interval);
          }
          else if (y_read > 768) {
            y_state = (y_state - 1) % 3;
            if (y_state < 0) { y_state = y_state + 3; }
            prev_input = curr_input;
            interval -= 10;
            if (interval <1) {interval = 1;}
            lcdMenu(state,interval);
          }
          else if (y_read > 600) {
            y_state = (y_state + 1) % 3;
            prev_input = curr_input;
            interval -= 1;
            if (interval <1) {interval = 1;}
            lcdMenu(state,interval);
          }
          delay(200);
          break;
        case 2:  //reset
          if (sw_read==0) {_reset();}
          lcdMenu(state,interval);
      }
    }
  }
}
void _reset() {
  digitalWrite(actuator1, HIGH);

  delay(2000);

  digitalWrite(actuator1, LOW);
}