// Needed to make NodeMCU work
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <Arduino.h>
#include <Wire.h>
#include <Framebuffer_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Led definitions
#define LED_COUNT 294
#define PIN D2

#define mw 21
#define mh 14
#define NUMMATRIX (mw*mh)

CRGB matrixleds[NUMMATRIX];

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(
  matrixleds,
  mw,
  mh,
  1,
  1,
  NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG);

const uint16_t colors[] = {
  matrix->Color(255, 0, 0),
  matrix->Color(0, 255, 0),
  matrix->Color(0, 0, 255)
};


// Adafruit_NeoPixel strip(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

// Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(21, 14, PIN,
//   NEO_MATRIX_BOTTOM  + NEO_MATRIX_RIGHT +
//   NEO_MATRIX_ROWS    + NEO_MATRIX_ZIGZAG,
//   NEO_GRB            + NEO_KHZ800);

// const uint16_t colors[] = {
//   matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

// Vars
bool ledsOn = true;

// Wifi
const char* wifi_ssid     = WIFI_SSID;
const char* wifi_password = WIFI_PASS;

// Web Server
ESP8266WebServer server(80);

String htmlContent = "\
  <html>\
    <title>Home Light Panel - Main page</title>\
    <body>\
    <h1>POST form data to /postform/</h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/postform/\">\
      <input type=\"text\" name=\"hello\" value=\"world\"><br>\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
    </body>\
  </html>\
";

void handleRoot() {
  digitalWrite(LED_BUILTIN, LOW);

  server.send(200, "text/html", htmlContent);

  digitalWrite(LED_BUILTIN, HIGH);
}

void handleLedsOn() {
  ledsOn = true;

  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void handleLedsOff() {
  ledsOn = false;

  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
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

/*******************************************************************/

/*
 * Setup
 */

void setup() {
  // Set onboard led pin
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(115200);


  /*
   *  Wifi
   */

  if(false) {
    Serial.print("Connecting to ");
    Serial.println(wifi_ssid);

    /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
      would try to act as both a client and an access-point and could cause
      network-issues with your other WiFi-devices on your WiFi-network. */
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());


    /*
    * Web Server
    */

    // Routes
    server.on("/", handleRoot);
    server.on("/on", handleLedsOn);
    server.on("/off", handleLedsOff);
    server.onNotFound(handleNotFound);

    // Server
    server.begin();
    Serial.println("HTTP server started");
  }

  /*
   * Leds
   */
  if(true) {
    FastLED.addLeds<WS2812B, PIN, GRB>(matrixleds, NUMMATRIX);

    // Start matrix
    matrix->begin();
    matrix->setTextWrap(false);
    matrix->setBrightness(5);
    matrix->setTextColor(colors[0]);

    // matrix.begin();
    // matrix.setTextWrap(false);
    // matrix.setBrightness(5);
    // matrix.setTextColor(colors[0]);
    // strip.begin();
    // strip.setBrightness(5);
    // strip.show();

    Serial.println("Strip LEDs started");
  }


  Serial.println("=========== Init");
}

/*******************************************************************/

/*
 * Functions
 */

int ledIndex = 0;
bool ledUp = true;

void goAndBackTestLights() {

  Serial.println("");
  Serial.print("goAndBackTestLights Sequence. ");
  Serial.print(ledIndex);
  Serial.println("");

  matrixleds[ledIndex] = CRGB::Red;
  FastLED.show();
  delay(10);

  matrixleds[ledIndex] = CRGB::Black;
  FastLED.show();
  delay(5);

  if (ledUp) {
    ledIndex++;
  }

  if (!ledUp) {
    ledIndex--;
  }

  // Change direction
  if(ledIndex == NUMMATRIX) {
    ledUp = false;
  }

  if(ledIndex == 0) {
    ledUp = true;
  }

}

/*******************************************************************/

/*
 * Loop
 */

int x    = mw;
int pass = 0;

void loop() {
  Serial.println("=========== Start Loop");

  // server.handleClient();

  // digitalWrite(LED_BUILTIN, LOW);
  // delay(500);

  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(1000);

  if(ledsOn) {
    goAndBackTestLights();
  }

  String text = "Alejandro Ferraiolo Mangione - Testing scrooling text!.";

  matrix->fillScreen(0);
  matrix->setCursor(x, 2);
  matrix->print(text);

  int textSize = text.length() * -6;

  Serial.println("===========");
  Serial.println(textSize);
  Serial.println("===========");

  // Character with space are 6 leds
  if(--x < textSize) {
    x = matrix->width();
    if(++pass >= 3) pass = 0;
    matrix->setTextColor(colors[pass]);
  }
  matrix->show();
  delay(150);

  Serial.println("=========== End Loop");
}
