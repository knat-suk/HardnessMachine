#include <Arduino.h>
#include "tft_lcd.h"
#include "motor_functions.h"
#include "LCD_functions.h"

const int sw = 42;
const int x = A1;
const int y = A0;

#define TFT_CS 22
#define TFT_RST 24
#define TFT_DC 26
#define TFT_MOSI 51
#define TFT_CLK 52
#define TFT_LED 32
#define TFT_MISO 50

#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

// Adafruit_FT6206 ctp = Adafruit_FT6206();

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

bool delta = false;
int speed = 0;
double curr_input = 0;
double prev_input = 0;
int y_state = 0;

void menuSetup(int state) {
  pinMode(x, INPUT_PULLUP);
  pinMode(y, INPUT_PULLUP);
  pinMode(sw, INPUT);
  digitalWrite(sw, HIGH);
  menuLoop(state);
}
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
            switch (y_state) {
              case 0:  //low torque
                changeInterval(1200);
                break;
              case 1:  //medium torque
                speed = 1000;
                break;
              case 2:  //high torque
                speed = 700;
                break;
            }
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
          if (sw_read==0) {lcdResetMotors(); moveManual();}
          lcdMenu(state,interval);
      }
    }
  }
}

void tftSetup(int state) {
  while (!Serial);
  Serial.begin(115200);
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);
  pinMode(x, INPUT_PULLUP);
  pinMode(y, INPUT_PULLUP);
  pinMode(sw, INPUT);
  digitalWrite(sw, HIGH);
  tft.begin();

  // if (! ctp.begin(40)) {  // pass in 'sensitivity' coefficient
  //   Serial.println("Couldn't start FT6206 touchscreen controller");
  //   while (1);
  // }

  tftMenu(state);


  while (state != -1) {
    // TS_Point p = ctp.getPoint();
    int x_read = analogRead(x);
    int y_read = analogRead(y);
    int sw_read = digitalRead(sw);
    // Serial.print("x = ");
    // Serial.println(x_read);
    // Serial.print("y = ");
    // Serial.println(y_read);
    // Serial.print("sw = ");
    // Serial.println(sw_read);
    // Serial.println("x_read= ");
    // Serial.println(x_read);
    // Serial.println("y_read= ");
    // Serial.println(y_read);
    // Serial.println("sw_read= ");
    // Serial.println(sw_read);
    delay(20);


    curr_input = millis();
    // lcdMenu(state, speed);
    tftMenu(state);
    // // p.x = map(p.x, 0, 240, 240, 0);
    // // p.y = map(p.y, 0, 320, 320, 0);
    // if (curr_input - prev_input > 200) {
    //   if (x_read > 768) {
    //     tft.fillScreen(ILI9341_BLACK);
    //     state = (state + 1) % 3;
    //     tftMenu(state);
    //     prev_input = curr_input;
    //     Serial.print("state= ");
    //     Serial.println(state);
    //   } else if (x_read < 256) {
    //     tft.fillScreen(ILI9341_BLACK);
    //     state = (state - 1) % 3;
    //     if (state < 0) { state += 3; }
    //     tftMenu(state);
    //     prev_input = curr_input;
    //     Serial.print("state= ");
    //     Serial.println(state);
    //   }
    //   switch (state) {
    //     case -1:
    //       Serial.println("run");
    //       delay(1000);
    //       state = 0;
    //     case 0:
    //       if (sw_read == 0) {
    //         tft.fillScreen(ILI9341_BLACK);

    //         switch (y_state) {
    //           tftMenu(state);
    //           case 0:  //low torque
    //             speed = 1200;
    //             break;
    //           case 1:  //medium torque
    //             speed = 1000;
    //             break;
    //           case 2:  //high torque
    //             speed = 700;
    //             break;
    //         }
    //         setup();
    //         state = -1;
    //       }
    //       break;
    //     case 1:  //settings
    //       if (y_read > 768) {
    //         y_state = (y_state + 1) % 3;
    //         prev_input = curr_input;
    //       } else if (y_read < 256) {
    //         y_state = (y_state - 1) % 3;
    //         if (y_state < 0) { y_state = y_state + 3; }
    //         prev_input = curr_input;
    //       }
    //       break;
    //     case 2:  //reset
    //       // resetMotors();
    //       tftMenu(state);
    //   }
    // }
    // tftMenu(state);
  }
}
String list[3] = { "Menu", "Settings", "Reset" };
String settings[3] = { "low", "medium", "high" };

void tftMenu(int state) {
  switch (state) {
    case 0:  // home
      tft.setCursor(26, 120);
      tft.setTextColor(ILI9341_RED);
      tft.drawFastHLine(0, 130, 2, ILI9341_BLUE);
      tft.setTextSize(3);
      tft.println(list[state]);

      tft.setCursor(20, 160);
      tft.setTextColor(ILI9341_GREEN);
      tft.setTextSize(2);
      tft.println("input");
    case 1:  // settings
      tft.setCursor(26, 120);
      tft.setTextColor(ILI9341_RED);
      tft.setTextSize(3);
      tft.println(list[state]);

      tft.setCursor(20, 160);
      tft.setTextColor(ILI9341_GREEN);
      tft.setTextSize(2);
      tft.println(settings[state]);
    case 2:  // reset
      tft.setCursor(26, 120);
      tft.setTextColor(ILI9341_RED);
      tft.setTextSize(3);
      tft.println(list[state]);

      tft.setCursor(20, 160);
      tft.setTextColor(ILI9341_GREEN);
      tft.setTextSize(2);
      tft.println("input");
  }
}
