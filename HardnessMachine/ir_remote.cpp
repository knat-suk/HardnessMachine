#include <IRremote.hpp>

const int receiver = 26;
unsigned long key_value = 0;

IRrecv irrecv(receiver);
decode_results results;


void irRemoteSetup() {
  irrecv.enableIRIn();
  irrecv.blink13(true);
}

void irRemoteLoop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    

    switch (results.value) {
      case 0:
        Serial.println("zero");
        break;
    }
    key_value = results.value;
    irrecv.resume();
  }
}
