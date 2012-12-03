#include <Arduino.h>

void setup() {
  // initialize serial:
  Serial.begin(38400);
  Serial.println("AT");
}

void loop() {
  Serial.println("XYZ"); 
  delay(1000);
}

