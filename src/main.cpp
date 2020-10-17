#include <Arduino.h>

// #define PIN 6

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);

  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  Serial.println("Loop");
  // Serial.println("Loop");
}