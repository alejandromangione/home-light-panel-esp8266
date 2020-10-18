#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
// #include <ESP8266WiFi.h>


// Led definitions
#define LED_COUNT 150
#define PIN D2

const char* wifi_ssid     = WIFI_SSID;
const char* wifi_password = WIFI_PASS;

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

    Serial.println(wifi_ssid);
    Serial.println(wifi_password);

    strip.setPixelColor(i, strip.Color(255, 0, 0 ));
    strip.show();
    delay(500);

    strip.clear();
    strip.show();
    delay(50);
  }

  Serial.println("=========== End Loop");
}
