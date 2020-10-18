#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


// Led definitions
#define LED_COUNT 150
#define PIN D2

Adafruit_NeoPixel strip(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

// Wifi
const char* wifi_ssid     = WIFI_SSID;
const char* wifi_password = WIFI_PASS;

// Web Server
ESP8266WebServer server(80);

void handleRoot() {
  digitalWrite(LED_BUILTIN, LOW);
  server.send(200, "text/plain", "hello from esp8266!\r\n");
  digitalWrite(LED_BUILTIN, HIGH);
}

void handleNotFound() {
  digitalWrite(LED_BUILTIN, LOW);

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

  digitalWrite(LED_BUILTIN, HIGH);
}

/*
 * Setup
 */

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(115200);


  //  Wifi
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  // Web Server
  server.on("/", handleRoot);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");


  // Leds
  strip.begin();
  strip.setBrightness(5);
  strip.show();
  Serial.println("Strip LEDs started");

  Serial.println("=========== Init");
}


/*
 * Functions
 */

int ledIndex = 0;
bool ledUp = true;

void goAndBackTestLights() {

  Serial.println("goAndBackTestLights Sequence. ");
  Serial.print(ledIndex);

  strip.setPixelColor(ledIndex, strip.Color(255, 0, 0 ));
  strip.show();
  delay(50);

  strip.clear();
  strip.show();
  delay(10);

  if (ledUp) {
    ledIndex++;
  }

  if (!ledUp) {
    ledIndex--;
  }

  // Change direction
  if(ledIndex == strip.numPixels()) {
    ledUp = false;
  }

  if(ledIndex == 0) {
    ledUp = true;
  }

}


/*
 * Loop
 */

void loop() {
  Serial.println("=========== Start Loop");

  server.handleClient();

  // digitalWrite(LED_BUILTIN, LOW);
  // delay(500);

  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(1000);

  goAndBackTestLights();

  Serial.println("=========== End Loop");
}
