#include "globals.h"

int global_state = 0;
int audioPlaying = 0;
int audioVolume = 21;
int LED_RED = 0;
int LED_GREEN = 0;
int LED_BLUE = 0;

String menuData[13];
String menuWIFI[33];

// If wled_ip is not used in globals.cpp, you can omit this line
const char* wled_ip = "http://192.168.0.90/json/state";
