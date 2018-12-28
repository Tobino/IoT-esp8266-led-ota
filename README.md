# esp8266-led-ota
A simple http server led On/Off control with OTA 

## Install


## First Build

Change plateformio.ini with upload_port = /dev/ttyS0

provide correct SSID and password on `config/conf`

Wire up your esp8266 on raspberry according to setup and esp brand

`plateformio run -t upload`

## Build over OTA
Make sure plateformio.ini has a correct upload_port and http://upload_port respond 200

provide correct SSID and password on `config/conf`

`plateformio run -t upload`


Source: https://circuits4you.com/2018/02/05/esp8266-arduino-wifi-web-server-led-on-off-control
