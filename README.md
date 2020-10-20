# Light Panel with ESP8266 and WS2812B led strip
This is a personal poject with the purpose of building a light panel with a ESP8266 and two stris of addressed leds (WS2812B)

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
 > Need to restart your machine

## Todo
- [x] Set up the environment
- [x] Test lights
- [x] Connect to local Wifi
- [x] Set up webserver
- [ ] Create webpage to handle actions
  - [ ] Mod selector (Text, New Year, Test Leds (default), etc)
  - [ ] Create asset host
  - [ ] On/off led sequence
  - [ ] Read text sequence
- [x] Build light panel
- [x] Add matrix leds
- [ ] Add text feature
- [ ] Integrate to Google Assistant
- [ ] New Year Counter

## References
- ESP8266WiFi - https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
- ESP8266WebServer - https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer
- Matrix - https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/examples/matrixtest/matrixtest.ino