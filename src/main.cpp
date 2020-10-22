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

// Vars
/*
 * Modes:
 * 0. Zigzag light (default)
 * 1. Scrooling text
 * 2. Countdown Timer
 * 3. Lines Test
 */
int mode = 0;

boolean wifiEnabled = true;

String scroolingTextContent = "Hellow World!";
int scroolingTextSize = 1;
int x    = mw;
int pass = 0;

// Clock Interval
const long interval = 1000;
unsigned long previousMillis = 0;

// Wifi
const char* wifi_ssid     = WIFI_SSID;
const char* wifi_password = WIFI_PASS;

// Web Server
ESP8266WebServer server(80);

/*******************************************************************/

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
  htmlContent = htmlContent + "localStorage.scroolingTextSize = " + "\"" + scroolingTextSize + "\";";

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
            <option value="2">Countdown timer</option>
            <option value="3">Random Lines</option>
          </select>
        </div>
        <div class="mode-0"></div>
        <div class="mode-1 block" style="display: none;">
          <form method="post" enctype="application/x-www-form-urlencoded" action="/update">

            <label for="scroolingTextContent">Text content:</label>
            <input type="text" name="scroolingTextContent" value=""><br>

            <label for="scroolingTextSize">Text size:</label>
            <select name="scroolingTextSize">
              <option value="1">Normal</option>
              <option value="2">Big</option>
            </select>

            <input type="submit" value="Submit">
          </form>
        </div>
        <div class="mode-2"></div>
        <div class="mode-3"></div>
        <div class="mode-4"></div>
        <!-- Script -->
        <script>
          // Mode
          document.querySelector(`.mode-${localStorage.mode}`).style.display = 'block';
          document.querySelector('[name=modeSwitcher]').selectedIndex = localStorage.mode;
          document.querySelector('[name=modeSwitcher]').addEventListener('change', (e) => {
            window.location.href = `${window.location.href}mode?id=${e.target.value}`;
          });

          // Scrooling Text
          document.querySelector('[name=scroolingTextSize]').selectedIndex = Number(localStorage.scroolingTextSize) - 1;
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
    if(server.argName(i) == "scroolingTextSize") scroolingTextSize = server.arg(i).toInt();
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
  if(wifiEnabled) {
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
  }

  /*
   * Leds
   */
  FastLED.addLeds<WS2812B, PIN, GRB>(matrixleds, NUMMATRIX);

  // Start matrix
  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(4);
  matrix->setTextColor(matrix->Color(255, 0, 0));

  Serial.println("Strip LEDs started");

  Serial.println("=========== Init");
}

/*******************************************************************/

/*
 * Mods
 */

int ledIndex = 0;
bool ledUp = true;

void zigzagLedFX() {

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

const uint16_t colors[] = {
  matrix->Color(255, 0, 0),
  matrix->Color(0, 255, 0),
  matrix->Color(0, 0, 255)
};

void scrollingTextFX() {
    matrix->fillScreen(0);

    if(scroolingTextSize == 1) matrix->setCursor(x, 3);
    if(scroolingTextSize == 2) matrix->setCursor(x, 0);

    matrix->setTextSize(scroolingTextSize);
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

//

int textSize = 3;
int countdownTimerValue = 9;
int indexColor = 0;

const uint16_t countdownTimerColors[] = {
  matrix->Color(204, 102, 51), //  (2)
  matrix->Color(255, 0, 0), //  (3)
  matrix->Color(204, 51, 51)  //  (1)
};

void countdownTimerFX() {
  matrix->fillScreen(0);

  if(textSize == 3) matrix->setCursor(3, -4);
  if(textSize == 2) matrix->setCursor(6, 0);
  if(textSize == 1) matrix->setCursor(8, 4);

  matrix->setTextSize(textSize);
  matrix->print(countdownTimerValue);
  matrix->setTextColor(countdownTimerColors[indexColor]);
  matrix->show();

  delay(200); // run 3 times

  indexColor++;
  if(indexColor > 2) indexColor = 0;

  textSize--;

  if(textSize < 1) {
    textSize = 3;

    // Extra delay when the text is small
    delay(400);

    countdownTimerValue--;

    if(countdownTimerValue <= 0) countdownTimerValue = 9;
  }
}

//

const int vMax = 13; // Represents the base of the board
const int hMax = 20;

void randomLinesFX() {
    matrix->fillScreen(0);
    // matrix->setRotation(4);

    // matrix->drawLine(
    //   0,  // x0 -
    //   0,  // y0 -
    //   mw, // x1 - 21
    //   mh, // x2 - 14
    //   matrix->Color(0, 255, 0) // Color
    // );

    for(int i = 0; i < mw; i++) {
      matrix->drawFastVLine(
        i,
        vMax,
        random(0, vMax) * -1,
        matrix->Color(
          random(0, 255),
          random(0, 255),
          random(0, 255)
        )
      );
    }

    // matrix->drawFastVLine(0, 0, 12, matrix->Color(0, 255, 0));
    // matrix->drawFastVLine(1, 0, 12, matrix->Color(0, 255, 0));

    // Line 1
    // matrix->drawFastHLine(
    //   0,  // x0 -
    //   0,  // y0 -
    //   random(1, mh), // x2 - 14
    //   matrix->Color(0, 255, 0) // Color
    // );

    // Line 2
    // matrix->drawFastHLine(
    //   1,  // x0 -
    //   0,  // y0 -
    //   random(1, mh), // x2 - 14
    //   matrix->Color(0, 255, 0) // Color
    // );

    matrix->show();

    delay(300);
}

/*******************************************************************/

/*
 * Loop
 */

void loop() {
  // Serial.println("=========== Start Loop");

  // Check http requests
  if(wifiEnabled) server.handleClient();

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
    case 2:
      countdownTimerFX();
      break;
    case 3:
      randomLinesFX();
      break;
  }

  // Serial.println("=========== End Loop");
}
