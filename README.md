# Light Panel with ESP8266 and WS2812B led strip
This is a personal project with the purpose of building a light panel with a ESP8266 and two stris of addressed leds (WS2812B)

## Materials
- Board ESP8266 (NodeMCU)
- Leds (WS2812B)

## Softwares
- Visual Code
- PlatformIO

## Set up env

**On Mac**
- Open your profile `sudo vim ~/.zshrc`
    ```
    export WIFI_SSID=XXXXXXXXXXXX
    export WIFI_PASS=XXXXXXXXXXXX
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

## References
- Arduino - https://www.arduino.cc/reference/en/
- ESP8266WiFi - https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
- ESP8266WebServer - https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer
- Led - https://github.com/FastLED/FastLED
- Matrix - https://github.com/marcmerlin/FastLED_NeoMatrix
- GFX library - https://cdn-learn.adafruit.com/downloads/pdf/adafruit-gfx-graphics-library.pdf