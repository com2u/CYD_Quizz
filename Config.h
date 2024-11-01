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
  if (doc.containsKey("QuizzMode")) QuizzMode = doc["QuizzMode"].as<signed int>();
  if (doc.containsKey("initMenuItem")) initMenuItem = doc["initMenuItem"].as<String>();
  if (doc.containsKey("TFTRotation")) TFTRotation = doc["TFTRotation"].as<signed int>();
  if (doc.containsKey("TFTInvertColor")) TFTInvertColor = doc["TFTInvertColor"].as<signed int>();
  if (doc.containsKey("xCalibFactor")) xCalibFactor = doc["xCalibFactor"].as<float>();
  if (doc.containsKey("yCalibFactor")) yCalibFactor = doc["yCalibFactor"].as<float>();
  if (doc.containsKey("xIntercept")) xIntercept = doc["xIntercept"].as<float>();
  if (doc.containsKey("yIntercept")) yIntercept = doc["yIntercept"].as<float>();


  //Serial.print("Loaded serverName: ");
  //Serial.println(serverName);
  //Serial.print("Loaded accessToken: ");
  //Serial.println(accessToken);
  Serial.println((String) "initMenuItem: "+initMenuItem);
  Serial.println((String) "startQuizz: "+startQuizz);
  Serial.println((String) "QuizzMode: "+QuizzMode);
  Serial.println((String) "TFTRotation: "+TFTRotation);
  Serial.println((String) "TFTInvertColor: "+TFTInvertColor);
  Serial.println((String) "xCalibFactor: "+xCalibFactor);
  Serial.println((String) "yCalibFactor: "+yCalibFactor);
  Serial.println((String) "xIntercept: "+xIntercept);
  Serial.println((String) "yIntercept: "+yIntercept);


  return true;
}

bool saveConfig() {
  
  DynamicJsonDocument doc(1024);
  doc["serverName"] = "api.example.com";
  doc["accessToken"] = "128du9as8du12eoue8da98h123ueh9h98";
  doc["startQuizz"] = startQuizz;
  doc["TFTRotation"] = (String) TFTRotation;
  doc["TFTInvertColor"] = (String) TFTInvertColor;
  doc["QuizzMode"] = QuizzMode;
  doc["initMenuItem"] = initMenuItem;
  doc["xCalibFactor"] = (String) xCalibFactor;
  doc["yCalibFactor"] = (String) yCalibFactor;
  doc["xIntercept"] = (String) xIntercept;
  doc["yIntercept"]= (String) yIntercept;

  Serial.println((String) "initMenuItem: "+initMenuItem);
  Serial.println((String) "startQuizz: "+startQuizz);
  Serial.println((String) "QuizzMode: "+QuizzMode);
  Serial.println((String) "TFTRotation: "+TFTRotation);
  Serial.println((String) "TFTInvertColor: "+TFTInvertColor);
  Serial.println((String) "xCalibFactor: "+xCalibFactor);
  Serial.println((String) "yCalibFactor: "+yCalibFactor);
  Serial.println((String) "xIntercept: "+xIntercept);
  Serial.println((String) "yIntercept: "+yIntercept);

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }
  serializeJson(doc, configFile);
  Serial.println("Config saved");
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



  if (!loadConfig()) {
    Serial.println("Failed to load config");
  } else {
    Serial.println("Config loaded");
  }

  saveConfig();
}
