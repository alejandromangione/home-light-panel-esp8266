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


// Vars
/*
 * Modes:
 * 0. Zigzag light (default)
 * 1. Scrooling text
 */
int mode = 0;

String scroolingTextContent = "Hello World!";
int x    = mw;
int pass = 0;


// Wifi
const char* wifi_ssid     = WIFI_SSID;
const char* wifi_password = WIFI_PASS;

// Web Server
ESP8266WebServer server(80);

void handleRoot() {
  digitalWrite(LED_BUILTIN, LOW);

  String htmlContent = R"(
    <html>
      <head>
        <title>Home Light Panel - Main page</title>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link href="https://fonts.googleapis.com/css2?family=PT+Sans&display=swap" rel="stylesheet">
        <style>
          html, body {
            font-family: 'PT Sans', sans-serif;
          }
          .block {
            display: block;
            margin: 10px;
          }
        </style>
        <script>
  )";

  // Loading all values into the browser
  htmlContent = htmlContent + "localStorage.mode = " + "\"" + mode + "\";";
  htmlContent = htmlContent + "localStorage.scroolingTextContent = " + "\"" + scroolingTextContent + "\";";

  htmlContent = htmlContent + R"(
      </script>
      </head>
      <body>
        <h1>Alex - Light Panel</h1><br>
        <div class="block">
          <label for="modeSwitcher">Select Mode:</label>
          <select name="modeSwitcher">
            <option value="0">Zigzag light</option>
            <option value="1">Scrooling text</option>
          </select>
        </div>
        <div class="mode-0"></div>
        <div class="block mode-1" style="display: none;">
          <form method="post" enctype="application/x-www-form-urlencoded" action="/update">
            <label for="scroolingTextContent">Text content:</label>
            <input type="text" name="scroolingTextContent" value=""><br>
            <input type="submit" value="Submit">
          </form>
        </div>
        <!-- Scripts -->
        <script>
          document.querySelector(`.mode-${localStorage.mode}`).style.display = 'block';
          document.querySelector('[name=modeSwitcher]').selectedIndex = localStorage.mode;
          document.querySelector('[name=modeSwitcher]').addEventListener('change', (e) => {
            localStorage.mode = e.target.value;
            window.location.href = `${window.location.href}mode?id=${e.target.value}`;
          });

          document.querySelector('[name=scroolingTextContent]').value = localStorage.scroolingTextContent;
        </script>
      </body>
    </html>
  )";

  /*
  * Notes:
  * After some digging how to pass values to the webpage, I thought some methods such as
  * concatenating the variables on the htmlContent or passing the value as data values ex."data-mode="myValue" and
  * came up the idea of using localStore that way I don't need to pass the value to the server
  * all value will be handled by the browser, let's see how that works. I found an issue, the approch is 
  */

  server.send(200, "text/html", htmlContent);

  digitalWrite(LED_BUILTIN, HIGH);
}

void handleSwitchMode() {
  // Example of request /mode?id=0
  if(server.args() > 0) {
    mode =  server.arg(0).toInt();

    FastLED.clear();
    FastLED.show();
  }

  // Refirect to home
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void handleParams() {
  for (uint8_t i = 0; i < server.args(); i++) {
    if(server.argName(i) == "scroolingTextContent") scroolingTextContent = server.arg(i);
  }

  // Refirect to home
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
  server.on("/mode", handleSwitchMode);
  server.on("/update", handleParams);
  server.onNotFound(handleNotFound);

  // Server
  server.begin();
  Serial.println("HTTP server started");


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

void zigzagLedFX() {

  // Serial.println("");
  // Serial.print("zigzagLedFX Sequence. ");
  // Serial.print(ledIndex);
  // Serial.println("");

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

void scrollingTextFX() {
    matrix->fillScreen(0);
    matrix->setCursor(x, 2);
    matrix->print(scroolingTextContent);

    int textSize = scroolingTextContent.length() * -6;

    // Character with space are 6 leds
    if(--x < textSize) {
      x = matrix->width();
      if(++pass >= 3) pass = 0;
      matrix->setTextColor(colors[pass]);
    }
    matrix->show();
    delay(150);
}

/*******************************************************************/

/*
 * Loop
 */

void loop() {
  // Serial.println("=========== Start Loop");

  // Check http requests
  server.handleClient();

  // digitalWrite(LED_BUILTIN, LOW);
  // delay(500);

  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(1000);

  switch (mode) {
    case 0:
      zigzagLedFX();
      break;
    case 1:
      scrollingTextFX();
      break;
  }

  // Serial.println("=========== End Loop");
}
