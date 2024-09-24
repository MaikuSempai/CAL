// C++ code
//

#include <IRremote.hpp>
#define IR_RECEIVE_PIN 10
int leds[6] = {2, 3, 4, 5, 6, 7};
int sw_1 = 9;
int sw_2 = 8;
int d = 150;
int R = A1;
int G = A0;

void setup() {
  Serial.begin(9600); 
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); 
  for (int x = 0; x < 6; x++) { 
    pinMode(leds[x], OUTPUT);
  }
  pinMode(11, OUTPUT);
}

void loop() {
  int sw1_val = digitalRead(sw_1);
  int sw2_val = digitalRead(sw_2);

  if (sw1_val == 0 && sw2_val == 0) {
    Red();
  } 
  else if (sw1_val == 0 && sw2_val == 1) {
    Green();
    IR();
  } 
  else if (sw1_val == 1 && sw2_val == 1) {
    Red();
  }
  else {
    Red();
  }
}

void Running_LR() {
  for (int x = 0; x < 6; x++) { 
    digitalWrite(leds[x], HIGH);
    delay(d);
    digitalWrite(leds[x], LOW);
  }
}


void IR() {
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.decodedRawData == 0XEF10BF00) {//1
      digitalWrite(11, HIGH); 
    } 
    else if (IrReceiver.decodedIRData.decodedRawData == 0xEE11BF00) {//2
      digitalWrite(11, LOW); 
    }
    else if (IrReceiver.decodedIRData.decodedRawData == 0xED12BF00) {//3
      digitalWrite(11, LOW);
      Running_LR();
    }
    else if (IrReceiver.decodedIRData.decodedRawData == 0xEB14BF00) {//4
      Standby();
    }

    IrReceiver.resume(); 
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
  }
}
void Red(){
  digitalWrite(R, HIGH);
  digitalWrite(G, LOW);
}

void Green(){
  digitalWrite(R, LOW);
  digitalWrite(G, HIGH);
}

void Standby(){
  for (int x = 0; x < 6; x++) { 
    digitalWrite(leds[x], LOW);
    digitalWrite(11, LOW);
  }
}