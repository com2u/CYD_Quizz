#ifndef MQTT_H
#define MQTT_H  // Header guard to prevent multiple inclusion

#include <PubSubClient.h>
#include <ArduinoJson.h>

#ifndef MQTT_GLOBALS  // This ensures global variables are only defined once
#define MQTT_GLOBALS


const char* MQTTPath = "NotABomb/CYD/init";
const char* MQTTOutput = "NotABomb/Key/";

int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[150];
char topicChar[150];
int value = 0;
int reconnectCount = 0;
StaticJsonDocument<220>  doc;
String MQTTTopic = "";
String MQTTPayload = "";

#endif

void clearMQTTMessage(){
  MQTTTopic = "";
  MQTTPayload = "";
}


void mycallback(char* topic, byte* payload, unsigned int length) {
  //int outputNo = -1;
  //Serial.println("Callback triggered"); 
  uint8_t value;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  
  // Set MQTTTopic from the incoming topic
  MQTTTopic = String(topic);
  // Set MQTTPayload directly from the payload
  MQTTPayload = String((char*)payload, length);
  Serial.println((String) MQTTTopic+" Payload ["+MQTTPayload+"]");
  if (MQTTPayload == "RESTART"){
    ESP.restart();
  }
}

void reconnect() {
  // Loop until we're reconnected
  if (WiFi.status() != WL_CONNECTED) {
    initWIFI();
  }
  int mqttNo = 0;
  int mqttSize = 0;

  while (mqttSize < sizeof(mqtt_servers) ) {
    mqttSize += sizeof(mqtt_servers[mqttNo]);
    Serial.print(mqttNo);
    Serial.print(" MQTT Server :");
    Serial.println(mqtt_server);

    client.setServer( "116.203.60.216", mqttPort);
    //client.setCallback(callback);
    Serial.print("Connecting ");
    while (!client.connected() && reconnectCount < 3) {
      reconnectCount++;

      // Create a random client ID
      String clientId = "CYD_NotABomb-";
      clientId += String(random(0xffff), HEX);
      Serial.print("Attempting MQTT connection... ");
      Serial.print(clientId);
      if (client.connect(clientId.c_str(), "MQTTiot",  "iot6812")) {
        client.setCallback(mycallback);
        Serial.println("connected");
        reconnectCount = 0;
        // Once connected, publish an announcement...
        clientId.toCharArray(msg, clientId.length());
        client.publish(MQTTPath, "Init");
        client.subscribe((String(MQTTOutput) + "#").c_str());
        client.subscribe((String(MQTTOutput) + "*").c_str());
        Serial.print("Subscribe: ");
        Serial.println(MQTTOutput);
        break;
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(500);
      }
    }
    if (client.connected()) {
      break;
    }
    reconnectCount = 0;
    mqttNo++;
  }
}

void initMQTT() {
  reconnect();
  //Serial.print("MQTT Server :");
  //Serial.println(mqtt_server);
  //Serial.print("MQTT Path :");
  //Serial.println(MQTTPath);
  //client.setServer( mqtt_server, mqttPort);
  client.setCallback(mycallback);

}

void handleMQTT() {
  if (!client.connected()) {
    reconnect();
  }
  //Serial.print("~");
  client.loop();
}

String checkMQTTQuizz(){
  handleMQTT();
  if (MQTTTopic != "") {
    if (MQTTTopic == "NotABomb/Key/Keypad" ){
      if (MQTTPayload ==  "1") {
        return menuQuizzOption[0];
      } 
      if (MQTTPayload ==  "2") {
        return menuQuizzOption[1];
      }
      if (MQTTPayload ==  "3") {
        return menuQuizzOption[2];
      }
      if (MQTTPayload ==  "4") {
        return menuQuizzOption[3];
      }
      if (MQTTPayload ==  "5") {
        return menuQuizzOption[3];
      }
      if (MQTTPayload ==  "6") {
        return menuQuizzOption[5];
      }
      if (MQTTPayload ==  "A") {
        return menuQuizzOption[0];
      }
      if (MQTTPayload ==  "B") {
        return menuQuizzOption[1];
      }
      if (MQTTPayload ==  "C") {
        return menuQuizzOption[2];
      }
      if (MQTTPayload ==  "D") {
        return menuQuizzOption[3];
      }
      if (MQTTPayload ==  "EXIT") {
        return "EXIT";
      }
    } 
  }
  return "";
}



boolean sendMQTT(String message) {
  boolean done = false;
  if (!client.connected()) {
    reconnect();
  }
  if (client.connected()) {
    String topic = MQTTPath;
    
    Serial.print(topic);
    Serial.print(" Message: ");
    Serial.println(message);
    message.toCharArray(msg, message.length());
    topic.toCharArray(topicChar, topic.length());
    client.publish(topicChar, msg);
    done = true;
  }
  return done;
}


#endif // MQTT_H