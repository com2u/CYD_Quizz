#include <ArduinoJson.h>
#include "FS.h"
#include "SPIFFS.h"

#define FORMAT_SPIFFS_IF_FAILED true

void listAllFiles(){
 
  File root = SPIFFS.open("/");
 
  File file = root.openNextFile();
 
  while(file){
 
      Serial.print("FILE: ");
      Serial.println(file.name());
 
      file = root.openNextFile();
  }
 
}

bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }
  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  DynamicJsonDocument doc(1024);
  //JsonObject json = jsonBuffer.parseObject(buf.get());

  auto error = deserializeJson(doc, buf.get());
  if (error) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(error.c_str());
      return false;
  }
 
  
  const char* serverName = doc["serverName"];
  const char* accessToken = doc["accessToken"];
  if (doc.containsKey("startQuizz")) startQuizz = doc["startQuizz"].as<String>();
  if (doc.containsKey("TFTRotation")) TFTRotation = doc["TFTRotation"].as<signed int>();
  //Serial.print("Loaded serverName: ");
  //Serial.println(serverName);
  //Serial.print("Loaded accessToken: ");
  //Serial.println(accessToken);
  Serial.print("startQuizz: ");
  Serial.println(startQuizz);
  Serial.print("TFTRotation: ");
  Serial.println(TFTRotation);
  return true;
}

bool saveConfig() {

  DynamicJsonDocument doc(1024);
  doc["serverName"] = "api.example.com";
  doc["accessToken"] = "128du9as8du12eoue8da98h123ueh9h98";
  doc["startQuizz"] = "quizz1.json";
  doc["TFTRotation"] = "0";

  

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }
  serializeJson(doc, configFile);

  //json.printTo(configFile);
  return true;
}

void InitConfig() {
  Serial.println("Mounting FS...");

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("Failed to mount file system");
    return;
  }

  listAllFiles();

  if (!saveConfig()) {
    Serial.println("Failed to save config");
  } else {
    Serial.println("Config saved");
  }

  if (!loadConfig()) {
    Serial.println("Failed to load config");
  } else {
    Serial.println("Config loaded");
  }
}
