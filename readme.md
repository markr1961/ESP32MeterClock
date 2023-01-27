This is a playground project using the ESP32-2432S028 demo board with code from the LVLG Arduino examples and VolosR' TDisplayS3MeterClock
Github repo: https://github.com/markr1961/ESP32MeterClock

board description:  ESP32 Arduino LVGL WIFI & Bluetooth Development Board 2.8" 240*320 Smart Display Screen 2.8inch LCD TFT Module With Touch WROOM
As of 2023-01-04: $9.46 + $4.06 shipping from Sunton Store https://www.aliexpress.us/item/3256804315935867.html

drivers and ESP32-2432S028R download link： http://www.jczn1688.com/zlxz

As of the time of this write-up in Dec 2022, there was very little info on this board. One comment thread on Reddit that provided useful:
https://www.reddit.com/r/esp32/comments/ws6n4u/unknown_schematic_for_esp322432s028_board_anyone/

The basic graphical design was forked from VolosR' TDisplayS2MeterClock https://github.com/VolosR/TDisplayS3MeterClock
His repo has lots of helpful projects based on the LilyGo series of ESP32 boards. He also has an active YouTube channel with lots of projects. You should check him out.

A collection of HW docs can be found at https://github.com/markr1961/_ESP32_dev_board_docs in the ESP32-2432S028 folder.

Arduino info
Board manager:  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
Library:        TFT_sSPI

Arduino IDE settings:
Arduino board:  ESP32 dev module
Flash mode:     DIO
Flash size:     4MB (32Mb)
Arduino runs on Core 1
Events run on Core 1
PSRAM:          disabled

Board info:
BN:   Unknown board
VID:  1A86
PID:  7523
SN:   Upload any sketch to obtain it
