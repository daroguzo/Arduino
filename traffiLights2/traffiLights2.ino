int red = 7;
int yellow = 8;
int green = 9;

int red2 = 4;
int green2 = 5;

int buzzerPin = 3;     //Define buzzerPin


void setup() {
  // put your setup code here, to run once:
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  pinMode(red2, OUTPUT);
  pinMode(green2, OUTPUT);

  pinMode(buzzerPin, OUTPUT);       //Set buzzerPin as output

  Serial.begin(9600);

}

void loop() {
  char read_data;

  if (Serial.available())
  {
    read_data = Serial.read();
    if ( read_data == '1')
    {
      trafficLightsOn();
      Serial.println("Police ON");
    }
    else if ( read_data == '0')
    {
      while (true) {
        Serial.println("Police OFF");
        
        trafficLightsOn();
        delay(5000);
        trafficLightsYellow();
        delay(1000);

        crosswalkOn();
        trafficLightsOn();
        read_data = Serial.read();
        if(read_data == '1'){
          Serial.println("Police ON");
          break;
        }
      }
    }
  }
}


void beep(unsigned char delayms) {  //creating function
  analogWrite(buzzerPin, 1);   //Setting pin to high
  delay(delayms); //Delaying
  analogWrite(buzzerPin , 0); //Setting pin to LOW
  delay(delayms); //Delaying

}

void trafficLightsOn() {
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(green, HIGH);

  digitalWrite(red2, HIGH);
  digitalWrite(green2, LOW);
}

void trafficLightsYellow() {
  digitalWrite(red, LOW);
  digitalWrite(yellow, HIGH);
  digitalWrite(green, HIGH);

  digitalWrite(red2, HIGH);
  digitalWrite(green2, LOW);
}

void crosswalkOn() {
  digitalWrite(red, HIGH);
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);

  digitalWrite(red2, LOW);
  digitalWrite(green2, HIGH);
  for (int i = 0; i < 5; i++) {
    beep(100);
    delay(500);
  }
}
