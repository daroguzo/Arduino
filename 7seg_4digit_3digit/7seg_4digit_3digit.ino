#include <SoftwareSerial.h>


const int a = 2;
const int b = 3;
const int c = 4;
const int d = 5;
const int e = 6;
const int f = A5;
const int g = A4;
const int p = 9;
int dig1 = 10;
int dig2 = 11;
int dig3 = 12;

int del = 750;

unsigned long preMills = 0;

static int i = 0;

// 각 달의 끝 날짜
int monthday[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int month = 0;
int month1 = 0;
int month2 = 0;
int day = 0;
int day1 = 0;
int day2 = 0;

SoftwareSerial Serial7Segment(7, 8); //RX pin, TX pin

int cycles = 0;

#define APOSTROPHE  5
#define COLON       4                            // 점위치
#define DECIMAL4    3                            // DECIMAL4 = 왼쪽에서 네번째 점
#define DECIMAL3    2                            // DECIMAL3 = 왼쪽에서 세번째 점
#define DECIMAL2    1                            // DECIMAL2 = 왼쪽에서 두번째 점
#define DECIMAL1    0                            // DECIMAL1 = 왼쪽에서 첫번째 점

byte seconds = 0;
byte minutes = 0;
byte hours = 0;
boolean amTime = true;                            // amTime = true(오전), false(오후)
boolean colonOn = false;                          // colonOn = false(꺼짐), true(켜짐)
String str = "";
char tempString[100]; //Used for sprintf

void setup() {

  Serial.begin(9600);

  Serial7Segment.begin(9600);                      // Serial7Segment 초기화
  Serial7Segment.write('v');                       // 화면 초기화

  Serial7Segment.write(0x77);                 // Decimal, colon, apostrophe control command
  Serial7Segment.write(1 << DECIMAL2);        // Turns on colon, apostrophoe, and far-right decimal
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

void loop()
{
  unsigned long curMills = millis();

  if (Serial.available() > 0) {
    str = Serial.readString();

    int first = str.indexOf("/");
    int second = str.indexOf("/", first + 1);
    int third = str.indexOf("/", second + 1);
    int fourth = str.indexOf("/", third + 1);

    int strlength = str.length();


    String str1 = str.substring(0, first);
    String str2 = str.substring(first + 1, second);
    String str3 = str.substring(second + 1, third);
    String str4 = str.substring(third + 1, fourth);
    String str5 = str.substring(fourth + 1, strlength);

    hours = str1.toInt();
    minutes = str2.toInt();
    seconds = str3.toInt();
    month = str4.toInt();
    day = str5.toInt();
  }

  // 매 초마다 실행


  if (seconds > 59) {                           // seconds가 59보다 커지면(60초가 되면)
    seconds -= 60;                              // seconds 초기화해주고(-60),
    minutes++;                                  // minutes = 분을 1씩 증가함.
    if (minutes > 59) {                         // minutes가 59보다 커지면(60분이 되면)
      minutes -= 60;                            // minutes 초기화해주고(-60)
      hours++;

      // 24시간제
      if (hours > 23) {
        hours = 0;
        day++;
      }
    }
  }
  // 3digit display
  if (day > monthday[month]) {
    month++;
    day = 1;
    if (month == 12 and (day > 12)) {
      month = 1;
      day = 1;
    }
  }
  if (month > 12) {
    month = 1;
  }

  month1 = int(month / 10);
  month2 = month % 10;
  day1 = int(day / 10);
  day2 = day % 10;

  if (day > 10 and day1 == 0) {
    if (i % 2 == 0) {
      onLED(3, month2);
      delay(3);
      onLED(2, day1);
      delay(3);
      onLED(1, day2);
      delay(3);
      if (i % 2 == 0) {
        onDot();
        delay(3);
      } else {
        onDot();
        delay(3);
        clearLEDs();
      }
    } else {
      onLED(3, month1);
      delay(3);
      onLED(2, month2);
      delay(3);
      onLED(1, day1);
      delay(3);
      if (i % 2 == 0) {
        onDot2();
        delay(3);
      } else {
        onDot2();
        delay(3);
        clearLEDs();
      }
    }
  } else if (day > 10 and day1 != 0) {
    if (i % 2 == 0) {
      onLED(3, month2);
      delay(3);
      onLED(2, day1);
      delay(3);
      onLED(1, day2);
      delay(3);
      if (i % 2 == 0) {
        onDot();
        delay(3);
      } else {
        onDot();
        delay(3);
        clearLEDs();
      }
    } else {
      onLED(3, month1);
      delay(3);
      onLED(2, month2);
      delay(3);
      onLED(1, day1);
      delay(3);
      if (i % 2 == 0) {
        onDot2();
        delay(3);
      } else {
        onDot2();
        delay(3);
        clearLEDs();
      }
    }
  } else {
    onLED(3, month1);
    delay(3);
    onLED(2, month2);
    delay(3);
    onLED(1, day2);
    delay(3);

    if (i % 2 == 0) {
      onDot2();
      delay(3);
    } else {
      clearLEDs();
    }
  }

  sprintf(tempString, "%02d%02d", hours, minutes); // 분,초 숫자 -> 문자열로 변환
  Serial7Segment.print(tempString);                  // 이 문자열을 segment에 써줌.

  if (curMills - preMills > 1000) {
    // comma는 1초마다 점멸
    i++;
    if (colonOn == true) {                        // 점이 켜져있으면
      colonOn = false;                            // 점을 꺼주고,
      Serial7Segment.write(0x77);                 // Decimal, colon, apostrophe control command
      Serial7Segment.write((byte) 0);             // Turns off colon, apostrophoe, and all decimals
    }

    else {                                        // 점이 꺼져있으면
      colonOn = true;                             // 점을 켜주고,
      Serial7Segment.write(0x77);                 // Decimal, colon, apostrophe control command
      Serial7Segment.write(1 << DECIMAL2);        // far-right decimal(왼쪽에서 두번째꺼 . 켜줌.)
    }
    sprintf(tempString, "%02d월%02d일 %02d시%02d분%02d초", month, day, hours, minutes, seconds);
    Serial.println(tempString);
    seconds++;
    preMills = curMills;

  }

}

void onDot() {
  onDigit3();
  dot();
}

void onDot2() {
  onDigit2();
  dot();
}

void dot() {
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(p, HIGH);
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
