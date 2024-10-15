// Example for library:
// https://github.com/Bodmer/TJpg_Decoder

// This example renders a Jpeg file that is stored in an array within Flash (program) memory
// see panda.h tab.  The panda image file being ~13Kbytes.

#define USE_DMA

#ifndef CYD_JPEG_H
#define CYD_JPEG_H

// Debugging flag


#pragma once
#include <TJpg_Decoder.h>
#include <TFT_eSPI.h>
//#include "panda.h"
#include <HTTPClient.h>

// Include SD
#define FS_NO_GLOBALS
#include <FS.h>
#ifdef ESP32
  #include "SPIFFS.h" // ESP32 only
#endif


// Forward declaration
extern TFT_eSPI tft;

// Ring buffer size (adjust as needed, but keep it a power of 2)
//#define INITIAL_BUFFER_SIZE 25384
//#define BUFFER_GROWTH_FACTOR 2

// Buffer size for chunked decoding
#define JPEG_BUFFER_SIZE 30000

#ifdef USE_DMA
  uint16_t  dmaBuffer1[16*16]; // Toggle buffer for 16*16 MCU block, 512bytes
  uint16_t  dmaBuffer2[16*16]; // Toggle buffer for 16*16 MCU block, 512bytes
  uint16_t* dmaBufferPtr = dmaBuffer1;
  bool dmaBufferSel = 0;
#endif

/*

// This next function will be called during decoding of the jpeg file to render each
// 16x16 or 8x8 image tile (Minimum Coding Unit) to the TFT.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

#ifdef USE_DMA
  // Double buffering is used, the bitmap is copied to the buffer by pushImageDMA() the
  // bitmap can then be updated by the jpeg decoder while DMA is in progress
  if (dmaBufferSel) dmaBufferPtr = dmaBuffer2;
  else dmaBufferPtr = dmaBuffer1;
  dmaBufferSel = !dmaBufferSel; // Toggle buffer selection
  tft.pushImageDMA(x, y, w, h, bitmap, dmaBufferPtr); // Initiate DMA - blocking only if last DMA is not complete
  // The DMA transfer of image block to the TFT is now in progress...
#else
  // Non-DMA blocking alternative
  tft.pushImage(x, y, w, h, bitmap);  // Blocking, so only returns when image block is drawn
#endif
  // Return 1 to decode next block.
  return 1;
}
*/

// This next function will be called during decoding of the jpeg file to render each
// 16x16 or 8x8 image tile (Minimum Coding Unit) to the TFT.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // Return 1 to decode next block
  return 1;
}




void JPEG_init()
{

  // Initialise the TFT
  tft.begin();
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.fillScreen(TFT_BLACK);

#ifdef USE_DMA
  tft.initDMA(); // To use SPI DMA you must call initDMA() to setup the DMA engine
#endif

  // The jpeg image can be scaled down by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The colour byte order can be swapped by the decoder
  // using TJpgDec.setSwapBytes(true); or by the TFT_eSPI library:
  tft.setSwapBytes(true);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);
}



/*
void drawJPEG_SDCard()
{
  //Initialise SD before TFT
  if (!SD.begin(SD_CS)) {
    Serial.println(F("SD.begin failed!"));
    while (1) delay(0);
  }
  Serial.println("\r\nInitialisation done.");

  tft.fillScreen(TFT_RED);

  // Time recorded for test purposes
  uint32_t t = millis();

  uint16_t w = 0, h = 0;
  TJpgDec.getSdJpgSize(&w, &h, "/panda.jpg");
  Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);

  // Draw the image, top left at 0,0
  TJpgDec.drawSdJpg(0, 0, "/panda.jpg");

  // How much time did rendering take (ESP8266 80MHz 271ms, 160MHz 157ms, ESP32 SPI 120ms, 8bit parallel 105ms
  t = millis() - t;
  Serial.print(t); Serial.println(" ms");
  // Wait before drawing again
  delay(2000);
}

void drawJPEG()
{
  // Show a contrasting colour for demo of draw speed
  tft.fillScreen(TFT_RED);


  // Get the width and height in pixels of the jpeg if you wish:
  uint16_t w = 0, h = 0;
  TJpgDec.getJpgSize(&w, &h, panda, sizeof(panda));
  Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);

  // Time recorded for test purposes
  uint32_t dt = millis();

  // Must use startWrite first so TFT chip select stays low during DMA and SPI channel settings remain configured
  tft.startWrite();

  // Draw the image, top left at 0,0 - DMA request is handled in the call-back tft_output() in this sketch
  TJpgDec.drawJpg(0, 0, panda, sizeof(panda));

  // Must use endWrite to release the TFT chip select and release the SPI channel
  tft.endWrite();

  // How much time did rendering take (ESP8266 80MHz 262ms, 160MHz 149ms, ESP32 SPI 111ms, 8bit parallel 90ms
  dt = millis() - dt;
  Serial.print(dt); Serial.println(" ms");

  // Wait before drawing again
  delay(2000);
}
*/

// Custom input function for TJpgDec
uint32_t jpgRead(JDEC* jdec, uint8_t* buf, uint32_t len) {
    WiFiClient* client = (WiFiClient*)jdec->device;
    if (buf) {
        return client->read(buf, len);
    } else {
        client->flush();
        return client->available();
    }
}




// Load and display a JPEG image from a URL
void loadURLImage(String url) {
    tft.setTextFont(4);     
    tft.setCursor(0, 30);
    tft.fillRect(0, 25, tft.width()-1, 75, TFT_BLACK);  
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.println("   Loading ...");
    tft.drawRect(0, 25, tft.width()-1, 75, TFT_LIGHTGREY);   
    tft.setTextFont(2);
    url.replace("{baseURL}",baseURL);
    HTTPClient http;
    WiFiClient* stream;
    Serial.println(url);
    //tft.fillScreen(TFT_RED);

    //Serial.print("[HTTP] begin...\n");
    http.begin(url);

    //Serial.print("[HTTP] GET...\n");
    int httpCode = http.GET();

    if (httpCode > 0) {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK) {
            stream = http.getStreamPtr();

            tft.fillScreen(TFT_BLACK);

            uint32_t dt = millis();

            // Buffer for storing chunks of the JPEG
            uint8_t* chunk = (uint8_t*)malloc(JPEG_BUFFER_SIZE);
            if (!chunk) {
                Serial.println("Chunk memory allocation failed");
                return;
            }

            // Start the decoder
            TJpgDec.drawJpg(0, 0, nullptr, 0);  // Initialize the decoder

            size_t totalBytes = 0;
            size_t bytesRead = 0;

            tft.startWrite();
            while (stream->available()) {
                bytesRead = stream->readBytes(chunk, JPEG_BUFFER_SIZE);
                totalBytes += bytesRead;

                if (DEBUG_OUTPUT) {
                    Serial.printf("Read %d bytes, total: %d\n", bytesRead, totalBytes);
                }

                // Feed the decoder with the new chunk
                if (TJpgDec.drawJpg(0, 0, chunk, bytesRead) != 1) {
                    Serial.println("JPEG decoding error");
                    break;
                }
            }
            tft.endWrite();

            free(chunk);

            dt = millis() - dt;
            Serial.printf("Decoding time: %lu ms Total bytes processed: %d\n", dt, totalBytes);


        }
    } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
}

#endif // CYD_JPEG_H