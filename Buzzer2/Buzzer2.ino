int buzzerPin = 3;     //Define buzzerPin
// 물결표시만 아날로그 입력 가능.
void setup() {
  pinMode(buzzerPin, OUTPUT);       //Set buzzerPin as output
  beep(50); //Beep
  beep(50); //Beep
  delay(1000); //Add a little delay

}

void loop() {
  beep(50); //Beep every 500 milliseconds
  delay(500);
}

void beep(unsigned char delayms) {  //creating function
  analogWrite(buzzerPin, 20);   //Setting pin to high
  delay(delayms); //Delaying
  analogWrite(buzzerPin ,0);  //Setting pin to LOW
  delay(delayms); //Delaying
  
}
