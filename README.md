# Light Panel with ESP8266 and WS2812B led strip
This is a personal project with the purpose of building a light panel with a ESP8266 and two stris of addressed leds (WS2812B)

## Materials
- Board ESP8266 (NodeMCU)
- Leds (WS2812B)

## Softwares
- VS Code
- PlatformIO

## Set up env

**On Mac**
- Open your profile `sudo vim ~/.zshrc`
    ```
    export WIFI_SSID=XXXXXXXXXXXX
    export WIFI_PASS=XXXXXXXXXXXX
    export ZIP_CODE=XX
    export OPENWEATHERMAP_APPID=XXXXXXXXXXX
    ```
 > Needs to restart your machine

## Todo
- [x] Set up the environment
- [x] Test lights
- [x] Connect to local Wifi
- [x] Set up webserver
- [x] Create webpage to handle actions
  - [x] Mod selector (Text, New Year, Test Leds (default), etc)
  - [x] Read text sequence
  - [ ] Enhance page
- [x] Build light panel
- [x] Add matrix leds
   - [x] Migrate to use FastLed. Based on (https://github.com/marcmerlin/FastLED_NeoMatrix)
- [x] Add text feature
- [ ] Integrate to Google Assistant
- [ ] Merry Christmas mode
    Use GIF? https://github.com/marcmerlin/AnimatedGIFs
- [ ] New Year Counter
    - [x] Countdown
- [ ] Upload Project photos
- [ ] Temperature
- [ ] Clock
   - https://randomnerdtutorials.com/interrupts-timers-esp8266-arduino-ide-nodemcu/
   - http://worldtimeapi.org/api/timezone/America/Vancouver
- [] Matrix background
- [] Microphone Status Mode

## References
- Arduino - https://www.arduino.cc/reference/en/
- ESP8266WiFi - https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
- ESP8266WebServer - https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer
- Led - https://github.com/FastLED/FastLED
- Matrix - https://github.com/marcmerlin/FastLED_NeoMatrix
- GFX library - https://cdn-learn.adafruit.com/downloads/pdf/adafruit-gfx-graphics-library.pdf
- Google Spreadsheet - https://www.benlcollins.com/apps-script/google-apps-script-beginner-guide/


## Misc
https://healthfully.com/128180-weight-loss-apps.html

## Matrix

4,6|4,7|4,8|4,9|4,11|4,15|4,16|4,17|5,6|5,9|5,11|5,15|6,6|6,7|6,8|6,9|6,11|6,15|6,16|7,6|7,9|7,11|7,12|7,13|7,15|7,16|7,17|

"0,0|0,1|0,2|0,3|0,4|0,5|0,6|0,7|0,8|0,9|0,10|0,11|0,12|0,13|0,14|0,15|0,16|0,17|0,18|0,19|0,20|1,0|1,20|2,0|2,20|3,0|3,20|4,0|4,4|4,5|4,6|4,7|4,9|4,13|4,14|4,15|4,20|5,0|5,4|5,7|5,9|5,13|5,20|6,0|6,4|6,5|6,6|6,7|6,9|6,13|6,14|6,20|7,0|7,4|7,7|7,9|7,10|7,11|7,13|7,14|7,15|7,20|8,0|8,20|9,0|9,6|9,7|9,8|9,9|9,11|9,13|9,20|10,0|10,8|10,11|10,13|10,20|11,0|11,6|11,7|11,8|11,11|11,12|11,13|11,20|12,0|12,20|13,0|13,1|13,2|13,3|13,4|13,5|13,6|13,7|13,8|13,9|13,10|13,11|13,12|13,13|13,14|13,15|13,16|13,17|13,18|13,19|13,20|"

FULL SUN
```
0,0|0,10|1,0|1,1|1,10|2,0|2,2|2,9|2,10|3,0|3,3|3,8|3,10|4,0|4,4|4,7|4,10|5,0|5,5|5,6|5,10|6,0|6,10|7,0|7,10|8,0|8,10|9,0|9,10|10,0|10,10|11,0|11,10|12,0|12,10|13,0|13,10|
```

HALF SUN
```
9,5|9,10|9,15|10,6|10,14|11,10|12,8|12,9|12,10|12,11|12,12|13,5|13,7|13,8|13,9|13,10|13,11|13,12|13,13|13,15|
```

MIC 1 by Juliana Belloni
```
0,0|0,1|0,2|0,3|0,4|0,5|0,6|0,7|0,8|0,9|0,10|0,11|0,12|0,13|0,14|0,15|0,16|0,17|0,18|0,19|0,20|1,0|1,20|2,0|2,9|2,10|2,11|2,20|3,0|3,8|3,9|3,10|3,11|3,12|3,20|4,0|4,8|4,9|4,10|4,11|4,12|4,20|5,0|5,8|5,9|5,10|5,11|5,12|5,20|6,0|6,8|6,9|6,10|6,11|6,12|6,20|7,0|7,9|7,10|7,11|7,20|8,0|8,10|8,20|9,0|9,10|9,20|10,0|10,8|10,9|10,10|10,11|10,12|10,20|11,0|11,7|11,8|11,9|11,10|11,11|11,12|11,13|11,20|12,0|12,20|13,0|13,1|13,2|13,3|13,4|13,5|13,6|13,7|13,8|13,9|13,10|13,11|13,12|13,13|13,14|13,15|13,16|13,17|13,18|13,19|13,20|
```

MIC 2 by Juliana Belloni
```
0,0|0,1|0,2|0,3|0,4|0,5|0,6|0,7|0,8|0,9|0,10|0,11|0,12|0,13|0,14|0,15|0,16|0,17|0,18|0,19|0,20|1,0|1,20|2,0|2,8|2,9|2,20|3,0|3,7|3,8|3,9|3,12|3,14|3,20|4,0|4,6|4,7|4,8|4,9|4,13|4,15|4,20|5,0|5,3|5,5|5,6|5,7|5,8|5,9|5,14|5,16|5,20|6,0|6,3|6,4|6,5|6,6|6,7|6,8|6,9|6,14|6,16|6,20|7,0|7,3|7,4|7,5|7,6|7,7|7,8|7,9|7,14|7,16|7,20|8,0|8,3|8,5|8,6|8,7|8,8|8,9|8,14|8,16|8,20|9,0|9,6|9,7|9,8|9,9|9,13|9,15|9,20|10,0|10,7|10,8|10,9|10,12|10,14|10,20|11,0|11,8|11,9|11,20|12,0|12,20|13,0|13,1|13,2|13,3|13,4|13,5|13,6|13,7|13,8|13,9|13,10|13,11|13,12|13,13|13,14|13,15|13,16|13,17|13,18|13,19|13,20|
```