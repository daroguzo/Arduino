const int _a=2;
const int _b=3;
const int _c=4;
const int _d=5;
const int _e=9;
const int _f=8;
const int _g=7;
const int _dot=6;
int dig1=10;
int dig2=11;
int dig3=12;
void setup() {
  // put your setup code here, to run once:
  pinMode(_a,OUTPUT);
  pinMode(_b,OUTPUT);
  pinMode(_c,OUTPUT);
  pinMode(_d,OUTPUT);
  pinMode(_e,OUTPUT);
  pinMode(_f,OUTPUT);
  pinMode(_g,OUTPUT);
  pinMode(_dot,OUTPUT);
  pinMode(dig1, OUTPUT);
  pinMode(dig2, OUTPUT);
  pinMode(dig3, OUTPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(dig1, HIGH);
  digitalWrite(dig2, HIGH);
  digitalWrite(dig3, HIGH);
  digitalWrite(_e,HIGH);
  digitalWrite(_b,HIGH);
  digitalWrite(_c,HIGH);
  digitalWrite(_d,HIGH);
  digitalWrite(_g,HIGH);
  digitalWrite(_f,HIGH);
  digitalWrite(_a,HIGH);
 digitalWrite(_dot,HIGH);
  delay(1000);
}
