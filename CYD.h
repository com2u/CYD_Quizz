#include "FS.h"
#include "SD.h"


#define CYD_LED_BLUE 4
#define CYD_LED_RED 17
#define CYD_LED_GREEN 16





#ifdef __cplusplus
extern "C" {
#endif
  uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

#define LDR_PIN 34


void LDR_init(){
  //analogSetAttenuation(ADC_0db);

  pinMode(LDR_PIN, INPUT);

  int sensorValue = analogRead(LDR_PIN);
  Serial.println((String) "LDR Analog Read: "+sensorValue);

}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void readFile(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void SDCard_init(){
   //spi2 = SPIClass(VSPI);

   Serial.println((String) "Temperature:"+temprature_sens_read());
  //if (!SD.begin(SS, mySpi, 80000000)) {
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  listDir(SD, "/", 0);
  readFile(SD, "/test.txt");
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void SDCardEnd(){
  //SD.end();
  //spi2.end();
}



void setLEDValue(){
  analogWrite(CYD_LED_RED, LED_RED); 
  analogWrite(CYD_LED_GREEN, LED_GREEN);
  analogWrite(CYD_LED_BLUE, LED_BLUE);
}

void CYD_init(){
  pinMode(CYD_LED_RED, OUTPUT);
  pinMode(CYD_LED_GREEN, OUTPUT);
  pinMode(CYD_LED_BLUE, OUTPUT);
  LED_RED = 0;
  LED_GREEN = 0;
  LED_BLUE = 255;
  setLEDValue();
  //SDCard_init();
  LDR_init();
  
}



void CYD_LED_Green(){
  LED_RED = 255;
  LED_GREEN = 0;
  LED_BLUE = 255;
  setLEDValue();
}

void CYD_LED_Red(){
  LED_RED = 0;
  LED_GREEN = 255;
  LED_BLUE = 255;
  setLEDValue();
}

void CYD_LED_Yellow(){
  LED_RED = 0;
  LED_GREEN = 150;
  LED_BLUE = 250;
  setLEDValue();
}