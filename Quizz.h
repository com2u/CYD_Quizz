#include <Regexp.h>
#include <regex>
// Regexp by Nick Gammon form the Library manager




String Quizz(String quizzNo){
   Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
      String baseURL = "http://sunrisetradingsystems.com/data/";
      DynamicJsonDocument doc = fetchJsonFromUrl((String) baseURL+quizzNo);
      String imageURL = "";
      String mp3 = "";
      String expectedAnswer = "";
      int countdown = 999;
      String selection = "";
      String nextPassed = "";
      String nextFailed = "";
      // Access the parsed JSON data
      
    if (!doc.isNull()) {
      if (doc.containsKey("Image")) {
        imageURL = doc["Image"].as<String>();

      }
      
      if (doc.containsKey("Sound")) {
        mp3 = doc["Sound"].as<String>();

      }
      
      if (doc.containsKey("Answer")) {
        expectedAnswer = doc["Answer"].as<String>();

      }
      
      if (doc.containsKey("Countdown")) {
        countdown = doc["Countdown"].as<int>();

      }
      
      if (doc.containsKey("Selection")) {
        selection = doc["Selection"].as<String>();

      }
      
      if (doc.containsKey("NextPassed")) {
        nextPassed = doc["NextPassed"].as<String>();

      }
      
      if (doc.containsKey("NextFailed")) nextFailed = doc["NextFailed"].as<String>();

      

          if (DEBUG_OUTPUT > 1) Serial.print((String) "Quiz image: "+imageURL+" MP3: "+mp3+" Answer: "+expectedAnswer+" Countdown: "+countdown+" Selection: "+selection+" Next Passed: "+nextPassed+" Next Failed: "+nextFailed);
            
          if (imageURL.length() > 0) loadURLImage(imageURL);
          if (mp3.length() > 0) {
            playAudio(mp3);
            //delay(5000);
            while(hanlde_AudioESP32()){}
          } else {
            Serial.println("No Audio file defined");
          }
          
          if (selection == "keyboard" ){
            String answer = keyboard(false);
            answer.trim();  // Remove leading and trailing whitespace
            Serial.println("trim");
            // Convert expectedAnswer to std::string
            std::string expectedAnswerStd = expectedAnswer.c_str();
            Serial.println("expectedAnswerStd = expectedAnswer.c_str()");
            
            // Create a regex pattern for case-insensitive match
            std::regex pattern(expectedAnswerStd, std::regex_constants::icase);
            Serial.println("pattern(expectedAnswerStd");
            
            // Perform the regex match
            bool match = std::regex_search(answer.c_str(), pattern);
            Serial.println("std::regex_search");
            if (match) {
              Serial.println("Expected Answer correct!");
              return nextPassed;
              // Handle correct answer (e.g., return nextPassed;)
            } else {
              Serial.println("Wrong Answer");
              return nextFailed;
            }

/*
              MatchState ms;

              // what we are searching (the target)
              char buf[100] = "";
              strncpy(buf, answer.c_str(), sizeof(buf) - 1);
              buf[sizeof(buf) - 1] = '\0';  // Ensure null-termination
              ms.Target(buf);  // set its address
              Serial.println( buf);
              
              char result = ms.Match ("(?i)^password$");
              Serial.println(REGEXP_MATCHED);
              Serial.println((int) result);
              if (result == REGEXP_MATCHED){
                Serial.println("Expected Answer correct!");
                return nextPassed;
              } else {
                Serial.println("Wrong Anser");
                return nextFailed;
              }
             */
            /*
            if (regexMatch(expectedAnswer.c_str(), answer.c_str())) {
              Serial.println("Expected Answer correct!");
              return nextPassed;
            } else {
              Serial.println("Wrong Anser");
              return nextFailed;
            }*/
          } 
          
          
       
      } else {
        Serial.println("No Quizz found");
        return "";
      }
     
}