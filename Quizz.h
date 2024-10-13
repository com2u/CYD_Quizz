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
String Quizz(String quizzNo){
   Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
      String baseURL = "https://com2u.github.io/CYD_Quizz/data/";
      DynamicJsonDocument doc = fetchJsonFromUrl((String) baseURL+quizzNo);
      String imageURL = "";
      String mp3 = "";
      String expectedAnswer = "";
      int countdownSet = 999;
      String selection = "";
      String nextPassed = "";
      String nextFailed = "";
      String quizzText = "";
      // Access the parsed JSON data
      
    if (!doc.isNull()) {

      if (doc.containsKey("Image")) imageURL = doc["Image"].as<String>();
      if (doc.containsKey("Sound")) mp3 = doc["Sound"].as<String>();
      if (doc.containsKey("Answer")) expectedAnswer = doc["Answer"].as<String>();
      if (doc.containsKey("Countdown")) countdownSet = doc["Countdown"].as<int>();
      if (doc.containsKey("Selection")) selection = doc["Selection"].as<String>();
      if (doc.containsKey("NextPassed")) nextPassed = doc["NextPassed"].as<String>();
      if (doc.containsKey("NextFailed")) nextFailed = doc["NextFailed"].as<String>();
      if (doc.containsKey("Text")) quizzText = doc["Text"].as<String>();
      

      if (DEBUG_OUTPUT > 1) Serial.println((String) "Quiz Text: "+quizzText+" Image: "+imageURL+" MP3: "+mp3+" Answer: "+expectedAnswer+" Countdown: "+countdownSet+" Selection: "+selection+" Next Passed: "+nextPassed+" Next Failed: "+nextFailed);
          
      if (imageURL.length() > 0) { loadURLImage(imageURL); } else { CYD_TFT_clear(); }
      if (quizzText.length() > 0) {
        Serial.println((String) "Quiz Text: "+quizzText);
        tft.setTextFont(2);
        int maxLineChar = 33;
        if (quizzText.length() < 70) {
          tft.setTextFont(4);
          maxLineChar = 20;
        }
        
        tft.setCursor(0, 30);
        tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
         //TFT_BLACK, TFT_WHITE, TFT_LIGHTGREY
         //tft.println(quizzText);
        //tft.println("Hallo Welt!");
         //tft.setCursor(5, 60);
         //tft.setTextColor(TFT_BLACK, TFT_LIGHTGREY);
         displayWrappedText(quizzText,maxLineChar);
         //tft.setTextFont(2);
         //delay(1000);
         //Serial.println((String) "Quiz Text: Hallo Welt!");
      }
      if (mp3.length() > 0) {
        playAudio(mp3);
        while(hanlde_AudioESP32()){}
      } else {
        Serial.println("No Audio file defined");
      }
      if (countdownSet > 0) {
          countdown = countdownSet;
          nextCountMillis = millis()+1000;
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
        // Countdown reset
        countdown = 0;
        if (match) {
          Serial.println("Expected Answer correct!");
          return nextPassed;
          // Handle correct answer (e.g., return nextPassed;)
        } else {
          Serial.println("Wrong Answer");
          return nextFailed;
        }
      }
      // Countdown reset
      countdown = 0;
    } else {
      Serial.println("No Quizz found");
      return "";
    }
     
}