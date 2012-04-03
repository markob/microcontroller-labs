#include <SoftwareSerial.h>

int sensorPin = 2;
int ledPin = 13;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT); 
  pinMode(sensorPin, INPUT);
}

void loop() {
  // read the value from the sensor:
  int sensorValue = digitalRead(sensorPin);    
  if (sensorValue) {
    // turn the ledPin on
    digitalWrite(ledPin, HIGH);
  } else {  
    // turn the ledPin off:        
    digitalWrite(ledPin, LOW);   
  }                  
}

