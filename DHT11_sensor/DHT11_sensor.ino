#include <Dht11.h>
#define DHT11PIN 2

//dht11 DHT11;

void setup()
{
    Serial.begin(9600);
    Serial.println("DHT11 TEST PROGRAM ");
    Serial.print("LIBRARY VERSION: ");
    Serial.println(Dht11::VERSION);
    Serial.println();
}

void loop()
{
    static Dht11 sensor(DHT11PIN);

    switch (sensor.read()) {
    case Dht11::OK:
        Serial.print("Humidity (%): ");
        Serial.println(sensor.getHumidity());

        Serial.print("Temperature (C): ");
        Serial.println(sensor.getTemperature());
        break;

    case Dht11::ERROR_CHECKSUM:
        Serial.println("Checksum error");
        break;

    case Dht11::ERROR_TIMEOUT:
        Serial.println("Timeout error");
        break;

    default:
        Serial.println("Unknown error");
        break;
    }

    delay(2000);
}

void loop() 
{ 
    long duration, inches, cm;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(10); 
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH); 

    // convert the time into a distance
    inches = microsecondsToInches(duration);
    cm = microsecondsToCentimeters(duration);
    Serial.print(inches);
    Serial.print("inch, ");
    Serial.print(cm);
    Serial.println(" cm"); 

    delay(100); 
}
