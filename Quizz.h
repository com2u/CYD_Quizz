#include <Regexp.h>
#include <regex>
// Regexp by Nick Gammon form the Library manager

void displayWrappedText(String text, int maxWidth) {
  String currentLine = "";
  int lastSpace = 0;
  
  for (unsigned int i = 0; i < text.length(); i++) {
    currentLine += text[i];
    
    if (text[i] == ' ') {
      lastSpace = i;
    }
    
    if (currentLine.length() == maxWidth) {
      if (lastSpace > 0) {
        currentLine = currentLine.substring(0, lastSpace);
        i = lastSpace;
      }
      tft.println(currentLine);
      currentLine = "";
      lastSpace = 0;
    }
  }
  
  if (currentLine.length() > 0) {
    tft.println(currentLine);
  }
}


String Quizz(String quizzNo){
   Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
      String baseURL = "https://com2u.github.io/CYD_Quizz/data/";
      DynamicJsonDocument doc = fetchJsonFromUrl((String) baseURL+quizzNo);
      String imageURL = "";
      String mp3 = "";
      String expectedAnswer = "";
      int countdown = 999;
      String selection = "";
      String nextPassed = "";
      String nextFailed = "";
      String quizzText = "";
      // Access the parsed JSON data
      
    if (!doc.isNull()) {

      if (doc.containsKey("Image")) imageURL = doc["Image"].as<String>();
      if (doc.containsKey("Sound")) mp3 = doc["Sound"].as<String>();
      if (doc.containsKey("Answer")) expectedAnswer = doc["Answer"].as<String>();
      if (doc.containsKey("Countdown")) countdown = doc["Countdown"].as<int>();
      if (doc.containsKey("Selection")) selection = doc["Selection"].as<String>();
      if (doc.containsKey("NextPassed")) nextPassed = doc["NextPassed"].as<String>();
      if (doc.containsKey("NextFailed")) nextFailed = doc["NextFailed"].as<String>();
      if (doc.containsKey("Text")) quizzText = doc["Text"].as<String>();
      

      if (DEBUG_OUTPUT > 1) Serial.print((String) "Quiz Text: "+quizzText+" Image: "+imageURL+" MP3: "+mp3+" Answer: "+expectedAnswer+" Countdown: "+countdown+" Selection: "+selection+" Next Passed: "+nextPassed+" Next Failed: "+nextFailed);
          
      if (imageURL.length() > 0) { loadURLImage(imageURL); } else { CYD_TFT_clear(); }
      if (quizzText.length() > 0) {
        Serial.print((String) "Quiz Text: "+quizzText);
        tft.setTextFont(3);
        tft.setCursor(10, 30);
        tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
         //TFT_BLACK, TFT_WHITE, TFT_LIGHTGREY
         tft.println(quizzText);
         tft.setCursor(10, 60);
         displayWrappedText(quizzText,40);
         tft.setTextFont(2);
      }
      if (mp3.length() > 0) {
        playAudio(mp3);
        while(hanlde_AudioESP32()){}
      } else {
        Serial.println("No Audio file defined");
      }
      
      if (selection == "keyboard" ){
        String answer = keyboard(false);
        answer.trim();  // Remove leading and trailing whitespace
        Serial.println("trim");
        // Create a regex pattern for case-insensitive match directly from expectedAnswer
        std::regex pattern(expectedAnswer.c_str(), std::regex_constants::icase);
        Serial.println("Regex pattern created");
        // Perform the regex match
        bool match = std::regex_search(answer.c_str(), pattern);
        if (match) {
          Serial.println("Expected Answer correct!");
          return nextPassed;
          // Handle correct answer (e.g., return nextPassed;)
        } else {
          Serial.println("Wrong Answer");
          return nextFailed;
        }
      } 
    } else {
      Serial.println("No Quizz found");
      return "";
    }
     
}