//
// Tools -> Partition Schme --> Huge App (3MB, 1MB)
// for CYD (Cheap Yellow Display) Display resolution 240 x 320
// ESP32-2432S028
//



#include <TFT_eSPI.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <TJpg_Decoder.h>
#include "globals.h"
#include "CYD.h"
#include "CYD_AudioESP32.h"
#include "CYD_TFT.h"
#include "CYD_JPEG.h"
#include "CYD_Time.h"
#include "CYD_WIFI.h"
#include "PingMonitor.h"
#include "Config.h"



//#define PROJECT_NAME "ESP32_CYD_PING"
//const char* wled_ip = "http://192.168.0.90/json/state";
const uint16_t TFT_COLOR_BLACK = 0;
const uint16_t TFT_COLOR_WHITE = 655350;

int countdown = 100;
unsigned long nextCountMillis;
String selectedItem;

void codeForCore0Task(void *parameter)
{
  for (;;)
  {
  
    if (global_state == 4) {
      checkWiFiConnection();
      pingHosts();
      checkUrls();
      delay(3000); // Wait for 30 seconds before the next cycle
    } else {
      delay(50);
    }
  }
}


void setup() {
  audioPlaying = 0;
  audioVolume = 21;
  LED_RED = 0;
  LED_GREEN = 0;
  LED_BLUE = 0;
  global_state = -1;
  Serial.begin(115200);
  CYD_TFT_init();
  CYD_init();
  CYD_LED_Yellow();
  CYD_TFT_print("Connet WIFI ...", TFT_COLOR_BLACK, TFT_COLOR_WHITE);
  scanWIFINetwork();
  initWIFI();
  initTime();
  CYD_TFT_clear();
  String mytimestamp = getMyTime();
  //CYD_TFT_BigDisplay();
  CYD_TFT_print(mytimestamp, TFT_COLOR_BLACK, TFT_COLOR_WHITE);
  //Audio_Init();
  init_AudioESP32();
  //for (int i=0; i< 1000; i++){
  hanlde_AudioESP32();
  InitConfig();
  saveConfig();
  //  delay(10);
  //}
  
  //CYD_TFT_init();
  global_state = 0;
  showMenu();
  TaskHandle_t Core0Task;

   xTaskCreatePinnedToCore(
    codeForCore0Task,
    "Core 0 task",
    10000,
    NULL,
    1,
    &Core0Task,
    1);
}


void Countdown() {
  if (global_state == 2) {
    if (millis() >= nextCountMillis ) {
      Serial.println(countdown);
      CYD_TFT_Top();
      CYD_TFT_print((String) countdown, TFT_COLOR_BLACK, TFT_COLOR_WHITE);
      if (countdown > 0) {
        countdown--;
        nextCountMillis = millis()+1000;
      } else {
        global_state = 0;
      }
    }
  }
}

void CountUp() {
  if (global_state == 7) {
    if (millis() >= nextCountMillis ) {
      Serial.println(countdown);
      CYD_TFT_Top();
      CYD_TFT_print((String) countdown, TFT_COLOR_BLACK, TFT_COLOR_WHITE);
      countdown++;
      nextCountMillis = millis()+1000;
    }
  }
}

void clearMenuData(){
  for (int i = 0; i < (sizeof(menuData) / sizeof(menuData[0])); i++){
    menuData[i] = "";
  }
}

void loop() {
  //checkWiFiConnection();
  //pingHosts();
  //checkUrls();
  //delay(30); // Wait for 30 seconds before the next cycle
  
  Countdown();
  CountUp();
  //handle_Audio();
  hanlde_AudioESP32();
  selectedItem = handleMenu();
  if (selectedItem != "") {
    Serial.println((String) "Selected menu item: " + selectedItem+"  Status:"+global_state);    
    if (selectedItem == "Time") {
      String mytimestamp = getMyTime();
      CYD_TFT_clear();
      CYD_TFT_print(mytimestamp, TFT_COLOR_BLACK, TFT_COLOR_WHITE);
      return;
    }
    if (selectedItem == "Countdown") {
      global_state = 2;
      countdown = 100;
      nextCountMillis = millis()+1000;
      CYD_TFT_clear();
      return;
    }
    if (selectedItem == "Sound Board"){
      global_state = 3;
    }
    if (selectedItem == "Ping") {
      global_state = 4;
      CYD_TFT_clear();
      CYD_TFT_print("Ping ...", TFT_COLOR_BLACK, TFT_COLOR_WHITE);
      return;
    }
    if (selectedItem == "Count Up") {
      global_state = 7;
      countdown = 1;
      nextCountMillis = millis()+1000;
      CYD_TFT_clear();
      return;
    }
    if (selectedItem == "Bird"){
      //Audio_Init();
      playAudio(0);
      return;
    }
    if (selectedItem == "Dog"){
      playAudio(1);
      return;
    }
    if (selectedItem == "Cat"){
      playAudio(2);
      return;
    }
    if (selectedItem == "Bee"){
      playAudio(3);
      return;
    }
    if (selectedItem == "Car"){
      playAudio(4);
      return;
    }
    if (selectedItem == "Horn"){
      playAudio(5);
      return;
    }
    if (selectedItem == "Bus"){
      playAudio(6);
      return;
    }
    if (selectedItem == "<--"){
      global_state = 0;
      clearMenuData();
    }
    if (selectedItem == "Image"){
      global_state = 0;
      

      CYD_TFT_DrawImage();
      delay(3000);
      
      String imagelist[] = {
      "https://images.wallpaperscraft.com/image/single/landscape_planets_stars_160551_240x320.jpg",
      "https://images.wallpaperscraft.com/image/single/color_paint_rainbow_55289_240x320.jpg",
      "https://images.wallpaperscraft.com/image/single/rainbow_line_light_15335_240x320.jpg",
      "https://wallpapersmug.com/download/240x320/d0bdec/black-shapes-pattern-abstract.jpg",
      "https://images.wallpaperscraft.com/image/single/nature_landscape_winter_10733_225x300.jpg",
      "https://images.hdqwalls.com/download/anonymous-4k-ok-240x320.jpg"};
      int arraySize = sizeof(imagelist) / sizeof(imagelist[0]);

      for(int i = 0; i < arraySize; i++) {

        loadURLImage(imagelist[i]);
        delay(5000);
      
      }
      keyboard(false);
      
    }
    
    if (selectedItem == "Setup"){
      global_state = 10;
    }
    if (selectedItem == "Keyboard"){
      global_state = 21;
      Serial.println(keyboard(true));
    }
      if (selectedItem == "Volume +"){
      if (audioVolume < 21){
        audioVolume++;
      }
      setLEDValue();
    }     
    if (selectedItem == "Volume -"){
      if (audioVolume > 0){
        audioVolume--;
      }
      setLEDValue();
    }
    if (selectedItem == "Red +"){
      if (LED_RED < 255){
        LED_RED++;
      }
      setLEDValue();
    }     
    if (selectedItem == "Red -"){
      if (LED_RED > 0){
        LED_RED--;
      }
      setLEDValue();
    }     
    if (selectedItem == "Green +"){
      if (LED_GREEN < 255){
        LED_GREEN++;
      }
      setLEDValue();
    }     
    if (selectedItem == "Green -"){
      if (LED_GREEN > 0){
        LED_GREEN--;
      }
      setLEDValue();
    }   
          if (selectedItem == "Blue +"){
      if (LED_BLUE < 255){
        LED_BLUE++;
      }
      setLEDValue();
    }     
    if (selectedItem == "Blue -"){
      if (LED_BLUE > 0){
        LED_BLUE--;
      }
      setLEDValue();
    }  
    if (global_state == 10){
      menuData[0] = String(audioVolume);
      menuData[1] = String(audioVolume);
      menuData[2] = String(LED_RED);
      menuData[3] = String(LED_RED);
      menuData[4] = String(LED_GREEN);
      menuData[5] = String(LED_GREEN);
      menuData[6] = String(LED_BLUE);
      menuData[7] = String(LED_BLUE);
    }

    showMenu();
    Serial.println((String) "global_state:"+global_state+" audioVolume:"+audioVolume+" LED_RED:"+LED_RED+" LED_GREEN:"+LED_GREEN+" LED_BLUE:"+LED_BLUE);
    
  } 
  delay(10);  // Small delay to prevent excessive checking
  

}
