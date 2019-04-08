int buzzer = 8;

void setup() {
  // put your setup code here, to run once:
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(buzzer, HIGH); //buzzer ON
  delay(1000);

  digitalWrite(buzzer, LOW);  //Buzzer OFF
  delay(1000);
}
