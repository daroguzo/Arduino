const int a = 2;
const int b = 3;
const int c = 4;
const int d = 5;
const int e = 6;
const int f = 7;
const int g = 8;
const int p = 9;
int dig1 = 10;
int dig2 = 11;
int dig3 = 12;

int del = 750;

unsigned long preMills = 0;

static int i = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(g, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(p, OUTPUT);
  pinMode(dig1, OUTPUT);
  pinMode(dig2, OUTPUT);
  pinMode(dig3, OUTPUT);

}

void loop() {
  
  unsigned long curMills = millis();

    onLED(1, (i + 2) % 10);
    delay(5);
    onLED(2, (i + 1) % 10);
    delay(5);
    onLED(3, i % 10);
    delay(5);
 
  if (curMills - preMills > 1000) {
    i++;
    preMills = curMills;
  }
}
void timer() {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      for (int k = 0; k < 10; k++) {
        onLED(3, i);
        delay(5);
        onLED(2, j);
        delay(5);
        onLED(1, k);
        delay(5);
      }
    }
  }
  delay(10000);
}

void onLED(int digit , int number) {
  onLEDs();
  switch (digit) {
    case 1:
      onDigit1();
      break;
    case 2:
      onDigit2();
      break;
    case 3:
      onDigit3();
      break;
    default:
      break;
  }

  switch (number) {
    case 0:
      zero();
      break;
    case 1:
      one();
      break;
    case 2:
      two();
      break;
    case 3:
      three();
      break;
    case 4:
      four();
      break;
    case 5:
      five();
      break;
    case 6:
      six();
      break;
    case 7:
      seven();
      break;
    case 8:
      eight();
      break;
    case 9:
      nine();
      break;
    default:
      break;
  }
  delayMicroseconds(1000);
}

void onDigit1() {
  digitalWrite(dig1, LOW);
  digitalWrite(dig2, HIGH);
  digitalWrite(dig3, HIGH);
}

void onDigit2() {
  digitalWrite(dig1, HIGH);
  digitalWrite(dig2, LOW);
  digitalWrite(dig3, HIGH);
}

void onDigit3() {
  digitalWrite(dig1, HIGH);
  digitalWrite(dig2, HIGH);
  digitalWrite(dig3, LOW);
}

// LED Down
void clearLEDs() {
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(p, LOW);
}

// LED On
void onLEDs() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(p, HIGH);
}

void zero() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
  digitalWrite(p, LOW);
}

void one() {
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(p, LOW);
}

void two() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
  digitalWrite(p, LOW);
}

void three() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
  digitalWrite(p, LOW);
}

void four() {
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(p, LOW);
}

void five() {
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(p, LOW);
}

void six() {
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(p, LOW);
}

void seven() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
  digitalWrite(p, LOW);
}

void eight() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(p, LOW);
}

void nine() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(p, LOW);
}

void zeroToNine() {
  zero();
  delay(del);
  one();
  delay(del);
  two();
  delay(del);
  three();
  delay(del);
  four();
  delay(del);
  five();
  delay(del);
  six();
  delay(del);
  seven();
  delay(del);
  eight();
  delay(del);
  nine();
  delay(del);
}
