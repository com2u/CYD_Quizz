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
          
          String answer = keyboard(false);
          if (expectedAnswer == answer) {
            Serial.println("Expected Answer correct!");
            return nextPassed;
          }else {
            Serial.println("Wrong Anser");
            return nextFailed;
          }
          
          
       
      } else {
        Serial.println("No Quizz found");
        return "";
      }
     
}