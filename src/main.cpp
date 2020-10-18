#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_COUNT 150
#define PIN D2

Adafruit_NeoPixel strip(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  strip.begin();
  strip.setBrightness(5);
  strip.show();

  Serial.println("=========== Init");
}

void loop() {
  Serial.println("=========== Start Loop");

  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);

  for (int i = 0; i < strip.numPixels(); i++) {
    Serial.println(i);

    strip.setPixelColor(i, strip.Color(255, 0, 0 ));
    strip.show();
    delay(100);

    strip.clear();
    strip.show();
    delay(50);
  }

  Serial.println("=========== End Loop");
}
