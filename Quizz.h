#pragma once
#include "MQTT.h"  // Add this line to include MQTT.h
#include <Regexp.h>
#include <regex>
// Regexp by Nick Gammon form the Library manager




void displayWrappedText(String text, int maxWidth = 33) {
  String currentLine = "";
  String nextWord = "";
  
  for (unsigned int i = 0; i < text.length(); i++) {
    if (text[i] == ' ' || i == text.length() - 1) {
      if (i == text.length() - 1 && text[i] != ' ') {
        nextWord += text[i];
      }
      
      if (currentLine.length() + nextWord.length() > maxWidth) {
        tft.println((String) "  "+currentLine);
        currentLine = nextWord;
      } else {
        if (currentLine.length() > 0) {
          currentLine += " ";
        }
        currentLine += nextWord;
      }
      nextWord = "";
    } else {
      nextWord += text[i];
    }
  }
  
  if (currentLine.length() > 0) {
    tft.println((String) "  "+currentLine);
  }
}

String validateQuizz(bool passed, String nextPassed, String nextFailed){
  countdown = 0;
  tft.setTextFont(4);     
  tft.setCursor(0, 30);
  tft.fillRect(0, 25, tft.width()-1, 75, TFT_BLACK);
  
  String message = "";
  if (passed){
          message = "  CORRECT\n\r  ANSWER !";       
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          CYD_LED_Green();
          tft.println(message);
          tft.drawRect(0, 25, tft.width()-1, 75, TFT_LIGHTGREY);
          Serial.println(message);
          //sendMQTT(message);
          tft.setTextFont(2);
          QuizzPassedCount++;
          delay(700);
          return nextPassed;
        } else {
          message = "  INCORRECT\n\r ANSWER";
          tft.setTextColor(TFT_RED, TFT_BLACK);
          CYD_LED_Red();
          tft.println(message);
          tft.drawRect(0, 25, tft.width()-1, 75, TFT_LIGHTGREY);
          Serial.println(message);
          //sendMQTT(message);
          tft.setTextFont(2);
          QuizzFailCount++;
          delay(700);
          if (QuizzMode == 0){
            return nextFailed;
          } else {
            return nextPassed;
          }
          
        }
}

bool regexMatch(String text, String patternExp){
         std::regex pattern(patternExp.c_str(), std::regex_constants::icase);
        Serial.println("Regex pattern created");
        // Perform the regex match
        return std::regex_search(text.c_str(), pattern);
 
}
String Quizz(String quizzNo){
   Serial.println((String) "Quizz:"+quizzNo);
   Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap() );
      DynamicJsonDocument doc = fetchJsonFromUrl((String) baseURL+quizzNo);
      String imageURL = "";
      String mp3 = "";
      String expectedAnswer = "";
      int countdownSet = 0;
      String selection = "";
      String nextPassed = "";
      String nextFailed = "";
      String quizzText = "";
      String topic = "";
      String payload = "";
      // Access the parsed JSON data
      
    if (!doc.isNull()) {

      if (doc.containsKey("Image")) imageURL = doc["Image"].as<String>();
      if (doc.containsKey("Sound")) mp3 = doc["Sound"].as<String>();
      if (doc.containsKey("Answer")) expectedAnswer = doc["Answer"].as<String>();
      if (doc.containsKey("Countdown")) countdownSet = doc["Countdown"].as<int>();
      if (doc.containsKey("Selection")) selection = doc["Selection"].as<String>();
      if (doc.containsKey("NextPassed")) nextPassed = doc["NextPassed"].as<String>();
      if (doc.containsKey("NextFailed")) nextFailed = doc["NextFailed"].as<String>();
      if (doc.containsKey("Topic")) topic = doc["Topic"].as<String>();
      if (doc.containsKey("Payload")) payload = doc["Payload"].as<String>();
      if (doc.containsKey("Text")) quizzText = doc["Text"].as<String>();
      

      if (DEBUG_OUTPUT > 1) Serial.println((String) "Quiz Text: "+quizzText+" Image: "+imageURL+" MP3: "+mp3+" Answer: "+expectedAnswer+" Countdown: "+countdownSet+" Selection: "+selection+" Next Passed: "+nextPassed+" Next Failed: "+nextFailed);
          
      if (imageURL.length() > 0) { loadURLImage(imageURL); } else { CYD_TFT_clear(); }
      if (quizzText.length() > 0) {
        Serial.println((String) "Quiz Text: "+quizzText);
        tft.setTextFont(2);
        int maxLineChar = 33;
        if (quizzText.length() < 70) {
          tft.setTextFont(4);
          maxLineChar = 15;
        }
        
        tft.setCursor(0, 30);
        tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
        displayWrappedText(quizzText,maxLineChar);
      }
      if (mp3.length() > 0) {
        playAudio(mp3);
        while(hanlde_AudioESP32()){}
      } else {
        Serial.println("No Audio file defined");
      }
      if (countdownSet > 0) {
          Serial.println("Set Countdown");
          countdown = countdownSet;
          nextCountMillis = millis()+1000;
      }
      sendMQTT(topic, payload);
      if (selection == "keyboard" ){
        String answer = keyboard(false);
        answer.trim();  // Remove leading and trailing whitespace
        Serial.println("trim");
        bool match = regexMatch(answer, expectedAnswer);
        // Countdown reset
        return validateQuizz(match, nextPassed, nextFailed);
      } else if (selection == "option" ){
        Serial.println("Section options");
        if (doc.containsKey("Option")) {
          int itemCount = 0;
          Serial.println("Find options");
          JsonArray optionArray = doc["Option"].as<JsonArray>();
          for (JsonVariant option : optionArray) {
            String optionText = option.as<String>();
            Serial.println(optionText);
            menuQuizzOption[itemCount] = optionText;
            itemCount++;
            // Here you can add code to display the option or handle it as needed
          }
          for (int item = itemCount; item < 5; item++) {
            menuQuizzOption[item] = "";
          }
        }
        int OptionMenuPosition = 148;
        showMenu(false, OptionMenuPosition);
        clearMQTTMessage();
        String selectedItem = "";
        while(selectedItem == ""  ){
          selectedItem = handleMenu(OptionMenuPosition);
          if (selectedItem == "") {
            selectedItem = checkMQTTQuizz();
          }
          delay(50);
        }
        if (selectedItem == "EXIT") {
          Serial.println("Exit Quizz");
          countdown = 0;
          global_state = 0;
          return "";
        }
        return validateQuizz((expectedAnswer == selectedItem), nextPassed, nextFailed);
        
      } else if (selection == "Area" ){
        Serial.println("Check area");
        TS_Point p = CYD_Handle_Touch(0);
        while(p.z == 0  ){
          p = CYD_Handle_Touch(0);
          if (p.z > 0) {
            if (doc.containsKey("Area")) {
                int itemCount = 0;
                Serial.println((String) "Find selected area z:" + p.z + " x:" + p.x + " y:" + p.y);
                JsonArray optionArray = doc["Area"].as<JsonArray>();
                for (JsonVariant entry : optionArray) {
                    int x = 0;
                    int y = 0;
                    int width = 0;
                    int height = 0;
                    String link = "";
                    
                    // Changed doc[] to entry[] for accessing object properties
                    if (entry.containsKey("x")) x = entry["x"].as<signed int>();
                    if (entry.containsKey("y")) y = entry["y"].as<signed int>();
                    if (entry.containsKey("width")) width = entry["width"].as<signed int>();
                    if (entry.containsKey("height")) height = entry["height"].as<signed int>();
                    if (entry.containsKey("link")) link = entry["link"].as<String>();
                    
                    Serial.println((String) "Area " + link + " x:" + x + " y:" + y + 
                                 " width:" + width + " height:" + height);
                    
                    if ((p.x >= x) && (p.x < x+width) && (p.y >= y) && (p.y < y+height)) {
                        if(countdownSet > 0) {
                          String message = "  CORRECT\n\r  ANSWER !";       
                          tft.setTextColor(TFT_GREEN, TFT_BLACK);
                          CYD_LED_Green();
                          tft.println(message);
                          tft.drawRect(0, 25, tft.width()-1, 75, TFT_LIGHTGREY);
                          Serial.println(message);
                          //sendMQTT(message);
                          tft.setTextFont(2);
                          QuizzPassedCount++;
                          delay(700);
                        }
                        return link;
                    }
                }
            } else {
                Serial.println("No areas found");
            }
          }  
        }
        
        

        return "";
      }
      // Countdown reset
      countdown = 0;
    } else {
      Serial.println("No Quizz found");
      return "";
    }
     
}