#ifndef GLOBALS_H
#define GLOBALS_H

#pragma once

#include <Arduino.h>  // Include the Arduino library to define String

#define DEBUG_OUTPUT 2
#define PROJECT_NAME "CYD_QUIZZ"

extern int global_state;
extern int audioPlaying;
extern int audioVolume;
extern int TFTMutex;
extern int LED_RED;
extern int LED_GREEN;
extern int LED_BLUE;
extern const char* wled_ip;
extern int countdown;
extern unsigned long nextCountMillis;



extern String menuData[13];
extern String menuWIFI[33];
extern String menuQuizzOption[5];
extern String baseURLInterent;
extern String baseURLPi;
extern String baseURL;
extern String startQuizz;
extern int TFTRotation;
extern int TFTInvertColor;
extern int QuizzMode;
extern int QuizzFailCount;
extern int QuizzPassedCount;
extern float xCalibFactor;
extern float yCalibFactor;
extern float xIntercept;
extern float yIntercept;
extern String initMenuItem;
extern String ConnectedWIFI;
extern String ConnectedWIFIHostname;
extern String ConnectedMQTTServer;
extern String ConnectedMQTTUser;
extern String ConnectedMQTTPassword;
extern String ConnectedclientId;



#endif // GLOBALS_H
