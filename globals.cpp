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
int countdown = 0;
int QuizzMode=1;
int QuizzFailCount=0;
int QuizzPassedCount=0;
int TFTMutex = 0;
unsigned long nextCountMillis;
String menuQuizzOption[] = {"", "", "", "", ""};
//String baseURL = "https://com2u.github.io/CYD_Quizz/data/";
String baseURLInterent = "http://sunrisetradingsystems.com/data/";
String baseURLPi = "http://192.168.1.1/";
String baseURL = "http://192.168.1.1/";
String startQuizz = "quizz1.json";
int TFTRotation = 0;
int TFTInvertColor = 0;


  float xCalibFactor = 0.07;
  float yCalibFactor = 0.09;
  float xIntercept = -19.27;
  float yIntercept = -19.50;
  String initMenuItem = "";

