void setup() {
  Serial.begin(9600);
  Serial.println("Hello Arduino");

}

void loop() {
  char read_data;

  if(Serial.available()){
    read_data = Serial.read();
    Serial.print(read_data);
  }
  delay(10);

}
