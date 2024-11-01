//
// Display resolution 240 x 320

#ifndef CYD_TFT_H
#define CYD_TFT_H

#pragma once
#include <TFT_eSPI.h>
#include "CYD_JPEG.h"
#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include "globals.h"



#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33


SPIClass mySpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);
TFT_eSPI tft = TFT_eSPI();

unsigned long lastMenuChangeMillis;

// Calibration Values
  int xTopLeft = -1;
  int xTopRight = -1;
  int xBottomLeft = -1;
  int xBottomRight = -1;
  int yTopLeft = -1;
  int yTopRight = -1;
  int yBottomLeft = -1;
  int yBottomRight = -1;

  

int state = 0;  
String menuEntry[]  = {"Quizz", "Timer", "Test", "Ping", "Keyboard", "Setup"};
String menuTest[]  = {"Image", "Sound Board",  "Keyboard",  "Invert Color",  "Original Color", "REBOOT", "<--"};
String menuTimer[]  = {"Time", "Countdown",  "Count Up", "Alarm", "<--"};
String menuAudioEntry[] = {"Bird", "Dog", "Cat", "Bee", "Car", "Horn", "Bus", "<--"};
String menuSetupEntry[] = {"Volume +", "Volume -", "LED", "Quizz Setup", "Calibrate Touch", "Display Vertical", "Display 180", "<--"};
String menuQuizz[] = {"Startup Quizz", "Quizz Fail Count", "Quizz Mode Restart", "Quizz Reset", "<--"};

String menuLED[] = {"Red +", "Red -", "Green +", "Green -", "Blue +", "Blue -", "<--"};
String menuInfoEntry[] = {"Volume ", "Red ", "Green ", "Blue ", "<--"};
String WIFIEntry[] = {" ", " ", " ", " ", "<--"};
String emptyStrings = "                              ";

const int MENU_ENTRY_COUNT = sizeof(menuEntry) / sizeof(menuEntry[0]);
const int MENU_AUDIO_ENTRY_COUNT = sizeof(menuAudioEntry) / sizeof(menuAudioEntry[0]);
const int MENU_SETUP_ENTRY_COUNT = sizeof(menuSetupEntry) / sizeof(menuSetupEntry[0]);
const int MENU_INFO_ENTRY_COUNT = sizeof(menuInfoEntry) / sizeof(menuInfoEntry[0]);
const int MENU_TIMER_ENTRY_COUNT = sizeof(menuTimer) / sizeof(menuTimer[0]);
const int MENU_TEST_ENTRY_COUNT = sizeof(menuTest) / sizeof(menuTest[0]);
const int MENU_LED_ENTRY_COUNT = sizeof(menuLED) / sizeof(menuLED[0]);

String *currentMenu;
int menuSize;

void initMenu(){
  if (global_state == 3) {
    currentMenu = menuAudioEntry;
    menuSize = sizeof(menuAudioEntry) / sizeof(menuAudioEntry[0]);
  } else if (global_state == 10) {
    currentMenu = menuSetupEntry;
    menuSize = sizeof(menuSetupEntry) / sizeof(menuSetupEntry[0]);
  } else if (global_state == 20) {
    currentMenu = menuInfoEntry;
    menuSize = sizeof(menuInfoEntry) / sizeof(menuInfoEntry[0]);
  } else if (global_state == 30) {
    currentMenu = menuTimer;
    menuSize = sizeof(menuTimer) / sizeof(menuTimer[0]);
  } else if (global_state == 40) {
    currentMenu = menuTest;
    menuSize = sizeof(menuTest) / sizeof(menuTest[0]);
  } else if (global_state == 50) {
    currentMenu = menuLED;
    menuSize = sizeof(menuLED) / sizeof(menuLED[0]);
  } else if (global_state == 100) {
    currentMenu = menuQuizzOption;
    menuSize = sizeof(menuQuizzOption) / sizeof(menuQuizzOption[0]);
  } else if (global_state == 110) {
    currentMenu = menuQuizz;
    menuSize = sizeof(menuQuizz) / sizeof(menuQuizz[0]);
  } else {
    currentMenu = menuEntry;
    menuSize = sizeof(menuEntry) / sizeof(menuEntry[0]);
  }
  
}


const char* keys[] = {
  "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
  "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
  "A", "S", "D", "F", "G", "H", "J", "K", "L", "Z",
  "Z", "X", "C", "V", "B", "N", "M", ".", "-", "+",
  "  ^", "   ", " <-","Enter","@"
};

void CYD_TFT_DrawImage(){
  //drawJPEG();  
}

void CYD_TFT_InvertColor(int value){
  tft.invertDisplay(value);
}


void mapTouchToDisplay(int touchX, int touchY, int &mappedX, int &mappedY) {
  // Map the x coordinate using linear interpolation
  float xFactorTop = (float)(tft.width() - 10) / (xTopRight - xTopLeft);
  float xFactorBottom = (float)(tft.width() - 10) / (xBottomRight - xBottomLeft);
  xIntercept = 10 - (xTopLeft * xFactorTop);
  
  // Map the y coordinate using linear interpolation
  float yFactorLeft = (float)(tft.height() - 10) / (yBottomLeft - yTopLeft);
  float yFactorRight = (float)(tft.height() - 10) / (yBottomRight - yTopRight);
  yIntercept = 10 - (yTopLeft * yFactorLeft);
  xCalibFactor = (xFactorTop + xFactorBottom) / 2.0;
  yCalibFactor = (yFactorLeft + yFactorRight) / 2.0;
  // Compute mapped coordinates
  mappedX = (touchX * xCalibFactor) + xIntercept;
  mappedY = (touchY * yCalibFactor) + yIntercept;
  Serial.println((String) "X Calibaration values xCalibFactor:"+xCalibFactor+" xIntercept:"+xIntercept );
  Serial.println((String) "Y Calibaration values yCalibFactor:"+yCalibFactor+" yIntercept:"+yIntercept );
}

TS_Point CYD_Handle_Touch(int calibrate) {
  if (ts.tirqTouched() && ts.touched()) {
    TS_Point p = ts.getPoint();
    if (calibrate == 1) {
      return p;
    }
    if (DEBUG_OUTPUT > 4) Serial.println((String) "Touch Original: X:"+p.x+" Y:"+p.y+" Z:"+p.z);
    /*
    int mappedX = (p.x * xCalibFactor) + xIntercept;
    int mappedY = (p.y * yCalibFactor) + yIntercept;
    p.x = (int) ((p.x-270) /15);
    p.y = (int) (p.y-120) /11.3;
    */
    p.x = (p.x * xCalibFactor) + xIntercept;
    p.y = (p.y * yCalibFactor) + yIntercept;
    //Serial.println((String) "Calibration X:"+mappedX+" Y:"+mappedY );
    if (DEBUG_OUTPUT > 3) Serial.println((String) "Touch: X:"+p.x+" Y:"+p.y+" Z:"+p.z);
    return p;
  }
  return TS_Point();  // Return an empty TS_Point if no touch detected
}

String keyboard(boolean clear) {
  String input = "";
  int shift = 1;
  //int keyWidth = 30;
  int keyHeight = 26;
  int keySpacing = 3;
  int startX = 5;
  int startY = 176;
  int cursorYpos = 153;
  int keysPerRow[] = {10, 10, 10, 10, 5};
  int rowOffsets[] = {0, 0, 0, 0, 0};
  int rowKeyWidth[] = {20, 20, 20, 20, 40};
  TFTMutex = 1;
  // Display keyboard
  if (clear) tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_LIGHTGREY );
  tft.setTextFont(2);

  int keyIndex = 0;
  for (int row = 0; row < 5; row++) {
    for (int col = 0; col < keysPerRow[row]; col++) {
      int x = startX + rowOffsets[row] + col * (rowKeyWidth[row] + keySpacing);
      int y = startY + row * (keyHeight + keySpacing);
      tft.fillRect(x, y, rowKeyWidth[row], keyHeight, TFT_LIGHTGREY);
      tft.drawRect(x, y, rowKeyWidth[row], keyHeight, TFT_BLACK);
      tft.setCursor(x + 5, y + 5);
      tft.print(keys[keyIndex]);
      keyIndex++;
    }
  }

  // Display input field
  tft.fillRect(5, cursorYpos, tft.width() - 10, 20, TFT_WHITE);
  tft.drawRect(5, cursorYpos, tft.width() - 10, 20, TFT_BLACK);
  startX = startX;
  startY = startY;
  TFTMutex = 0;
  while (true) {
    TS_Point p = CYD_Handle_Touch(0);
    if (p.z > 0) {
      int row = (p.y - startY) / (keyHeight + keySpacing);
      int col = (p.x - startX - rowOffsets[row]) / (rowKeyWidth[row] + keySpacing);

      if (row >= 0 && row < 5 && col >= 0 && col < keysPerRow[row]) {
        int keyIndex = 0;
        for (int r = 0; r < row; r++) {
          keyIndex += keysPerRow[r];
        }
        keyIndex += col;

        if (keyIndex < sizeof(keys) / sizeof(keys[0])) {
          String key = keys[keyIndex];

          if (key == "Enter") {
            return input;
          } else if (key == " <-") {
            if (input.length() > 0) {
              input = input.substring(0, input.length() - 1);
            }
          } else if (key == "   ") {
            input += " ";
          } else if (key == "  ^") {
            shift = shift * -1;
          } else {
            if (shift == 1) {
              input += key;
            } else {
              char lowerChar = key.charAt(0);
              if (lowerChar >= 'A' && lowerChar <= 'Z') {
                lowerChar = lowerChar - 'A' + 'a';
              }
              input += lowerChar;
            }
          }
          TFTMutex = 1;
          // Update input display
          tft.fillRect(10, cursorYpos+1, tft.width() - 20, 18, TFT_WHITE);
          tft.setCursor(15, cursorYpos+2);
          tft.print(input);
          TFTMutex = 0;
        }
      }

      delay(200); // Debounce
    }
  }
}

void touchScreen_Start(){
  mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  ts.begin(mySpi);
  ts.setRotation(TFTRotation);
}


void CYD_TFT_init(){
  tft.init();
  tft.setRotation(TFTRotation);
  tft.invertDisplay(TFTInvertColor);
  Serial.println((String) "TFTInvertColor: "+TFTInvertColor);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor( TFT_WHITE, TFT_BLACK);
  tft.setTextFont(2);
  touchScreen_Start();
  
  JPEG_init();
}





void CYD_TFT_BigDisplay(){
  tft.init();
  tft.setRotation(1);
  ts.setRotation(1);
  tft.setCursor(0, 0);
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextFont(4);

}






void showMenu(bool clearDisplay, int startY = 10) {
  if (clearDisplay) tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_BLACK, TFT_LIGHTGREY);
  tft.setTextFont(2);
  
  
  initMenu();
  if (DEBUG_OUTPUT > 3) Serial.println((String) "MenuSize: " + menuSize);
  int menuItemHeight = 30;
  int menuItemPadding = 5;
  //int startY = 10;
  
  for (int i = 0; i < menuSize; i++) {
    int y = startY + i * (menuItemHeight + menuItemPadding);
    tft.fillRect(10, y, tft.width() - 20, menuItemHeight, TFT_LIGHTGREY);
    tft.drawRect(10, y, tft.width() - 20, menuItemHeight, TFT_WHITE);
    tft.setCursor(15, y + (menuItemHeight / 2) - 6);
    tft.println((String) currentMenu[i]+" "+menuData[i]);
    if (DEBUG_OUTPUT > 3) Serial.println((String) currentMenu[i]+" "+menuData[i]);
  }
}

String checkMenuTouch(TS_Point p, int startY = 10) {

  // debounce
  if ((lastMenuChangeMillis+300) > millis()){
    if (DEBUG_OUTPUT > 3) Serial.println((String) "Touch deboucer "+(lastMenuChangeMillis+300)+" > "+millis());
     return "";
  }

  initMenu();

  int menuItemHeight = 30;
  int menuItemPadding = 5;
  
  
  for (int i = 0; i < menuSize; i++) {
    int y = startY + i * (menuItemHeight + menuItemPadding);
    if (p.y >= y && p.y <= y + menuItemHeight && p.x >= 10 && p.x <= tft.width() - 10) {
      lastMenuChangeMillis = millis();
      return String(currentMenu[i]);
    }
  }
  if (global_state == 7 || global_state == 4 || global_state == 3){
    global_state = 0;
  }
  
  return "";  // Return empty string if no menu item was touched
}


String handleMenu(int startY = 10){
  TS_Point p = CYD_Handle_Touch(0);
  if (p.z > 0) {  // Check if there's a valid touch
    return checkMenuTouch(p, startY);
  } else {
    return "";
  }
}

void handleCalibration(){

  
  TS_Point p = CYD_Handle_Touch(0);
  Serial.println("Wait clibrartion to exit");
  while(((p.x < 1000) || (p.x > 3000)) && ((p.y < 1000) || (p.y > 3000))){
    p = CYD_Handle_Touch(1);
    if (p.x != 0 && p.y != 0){
      Serial.println((String) "Touch: X:"+p.x+" Y:"+p.y+" Z:"+p.z);
      if ((p.x < 1000) && (p.y < 1000)){
        xTopLeft = p.x;
        yTopLeft = p.y;
      }
      if ((p.x > 3000) && (p.y < 1000)){
        xTopRight = p.x;
        yTopRight = p.y;
      }
      if ((p.x < 1000) && (p.y > 3000)){
        xBottomLeft = p.x;
        yBottomLeft = p.y;
      }
      if ((p.x > 3000) && (p.y > 3000)){
        xBottomRight = p.x;
        yBottomRight = p.y;
      }
      if (xTopLeft != -1 && xTopRight != -1 && xBottomLeft != -1 && xBottomRight != -1){
        Serial.println((String) "Expected pixel (x,y) 10,10 Touch: xTopLeft:"+xTopLeft+ ", yTopLeft:"+yTopLeft);
        Serial.println((String) "Expected pixel (x,y) 230,10 Touch: xTopRight:"+xTopRight+ ", yTopRight:"+yTopRight);
        Serial.println((String) "Expected pixel (x,y) 10,310 Touch: xBottomLeft:"+xBottomLeft+ ", yBottomLeft:"+yBottomLeft);
        Serial.println((String) "Expected pixel (x,y) 230,310 Touch: xBottomRight:"+xBottomRight+ ", yBottomRight:"+yBottomRight);
        tft.setCursor(60, 80);
        tft.print("Calibrated");
        int mappedX, mappedY;
        mapTouchToDisplay(p.x, p.y, mappedX, mappedY);
        Serial.println((String) "calibarated (x,y) "+ mappedX+ ","+mappedY);
      }
    }
    delay(10);
  }
  Serial.println("Calibartion completed");
  global_state = 0;

}


String Trim(String url) {
  // Convert to lowercase for case-insensitive comparison
  url.toLowerCase();
  
  // Remove "http://" or "https://"
  if (url.startsWith("http://")) {
    url = url.substring(7);
  } else if (url.startsWith("https://")) {
    url = url.substring(8);
  }
  
  // Remove "www." if present
  if (url.startsWith("www.")) {
    url = url.substring(4);
  }
  
  // Trim to 25 characters
  if (url.length() > 30) {
    url = url.substring(0, 27) + "...";
  }
  url = url+emptyStrings.substring(0,31-url.length());
  return url;
}


void CYD_TFT_Top(){
    tft.setTextFont(2);
    tft.setCursor(0, 0);
}

void CYD_TFT_clear(){
    tft.fillScreen(TFT_BLACK);
    tft.setTextFont(2);
    tft.setCursor(0, 0);
}

void CYD_TFT_print(String text, uint16_t fg_color, uint16_t bg_color) {
  tft.setTextColor(fg_color, bg_color);
  tft.println(Trim(text));
}

#endif