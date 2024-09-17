int sw1 = A0;
int sw2 = A1;
int sw3 = A2;
int sw4 = A3;
int delayVal = 200;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for(int x = 2; x <= 9; x++) {
    pinMode(x, OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int sw1val = digitalRead(sw1);
  int sw2val = digitalRead(sw2);
  int sw3val = digitalRead(sw3);
  int sw4val = digitalRead(sw4);

  if(sw1val == 0 && sw2val == 1 && sw3val == 0 && sw4val == 1) {
    RightLeft();
  }

  else if(sw1val == 1 && sw2val == 0 && sw3val == 1 && sw4val == 0) {
    LeftRight();
  }

  else {
    Standby();
  }
}

void LeftRight() {
  for(int x = 2; x <= 9; x++) {
    digitalWrite(x, HIGH);
    delay(delayVal);

    digitalWrite(x, LOW);
  }
}

void RightLeft() {
  for(int x = 2; x <= 10; x++) {
    digitalWrite(x, HIGH);
    delay(delayVal);
    digitalWrite(x-1, LOW);
  }
  for(int x = 9; x >= 3; x--) {
    digitalWrite(x, HIGH);
    delay(delayVal);
    digitalWrite(x, LOW);
  }
}

void Standby() {
  for(int x = 2; x <= 9; x++ ) {
    digitalWrite(x, LOW);
  }
}