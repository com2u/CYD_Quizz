#ifndef GLOBALS_H
#define GLOBALS_H

#pragma once

#include <Arduino.h>  // Include the Arduino library to define String

#define DEBUG_OUTPUT 2
#define PROJECT_NAME "ESP32_CYD_PING"

extern int global_state;
extern int audioPlaying;
extern int audioVolume;
extern int LED_RED;
extern int LED_GREEN;
extern int LED_BLUE;
extern const char* wled_ip;



extern String menuData[13];
extern String menuWIFI[33];

#endif // GLOBALS_H
