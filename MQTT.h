#pragma once
#ifndef MQTT_H
#define MQTT_H  // Header guard to prevent multiple inclusion

#include <PubSubClient.h>
#include <ArduinoJson.h>
//#include "Credentials.h"  // Include your credentials

#ifndef MQTT_GLOBALS  // This ensures global variables are only defined once
#define MQTT_GLOBALS

const char* MQTTPath = "NotABomb/CYD/init";
const char* MQTTOutput = "NotABomb/Key/";

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

void clearMQTTMessage() {
  MQTTTopic = "";
  MQTTPayload = "";
}

void mycallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("]");

  MQTTTopic = String(topic);
  MQTTPayload = String((char*)payload, length);
  Serial.println(MQTTTopic + " Payload [" + MQTTPayload + "]");
  if (MQTTPayload == "RESTART") {
    ESP.restart();
  }
}

void reconnect() {
  if (WiFi.status() != WL_CONNECTED) {
    initWIFI();
  }

  int mqttNo = 0;

  while (mqttNo < sizeof(mqtt_servers) / sizeof(mqtt_servers[0])) {
    Serial.print(mqttNo);
    Serial.print(" MQTT Server: ");
    ConnectedMQTTServer = mqtt_servers[mqttNo];
    Serial.println(ConnectedMQTTServer);

    client.setServer(mqtt_servers[mqttNo].c_str(), mqttPort);
    
    while (!client.connected() && reconnectCount < 3) {
      reconnectCount++;

      String clientId = "CYD_NotABomb-";
      clientId += String(random(0xffff), HEX);
      Serial.print(clientId);
      Serial.print(" MQTT --> ");
      Serial.print(ConnectedWIFI);
      Serial.print(" Broker: ");
      Serial.print(ConnectedMQTTServer);
      ConnectedMQTTUser = MQTTUsers[mqttNo];
      ConnectedMQTTPassword = MQTTPasswords[mqttNo];
      Serial.print(" User: ");
      Serial.print(ConnectedMQTTUser);
      Serial.print(" Password: ");
      Serial.println(ConnectedMQTTPassword);
      Serial.print(" ");
        
      ConnectedMQTTUser = MQTTUsers[mqttNo];
      ConnectedMQTTPassword = MQTTPasswords[mqttNo];
      
      if (client.connect(clientId.c_str(), MQTTUsers[mqttNo].c_str(), MQTTPasswords[mqttNo].c_str())) {
        ConnectedclientId = clientId;
        client.setCallback(mycallback);
        Serial.println("connected");
        reconnectCount = 0;
        clientId.toCharArray(msg, clientId.length());
        client.publish(MQTTPath, "Init");
        client.subscribe((String(MQTTOutput) + "#").c_str());
        client.subscribe((String(MQTTOutput) + "*").c_str());
        client.subscribe(String("NotABomb/Challenge").c_str());
        Serial.print("Subscribe: ");
        Serial.println(MQTTOutput);
        break;
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(500);
      }
    }

    if (client.connected()) {
      break;
    }
    ConnectedMQTTServer = "NONE!";

    reconnectCount = 0;
    mqttNo++;
  }
}

void initMQTT() {
  reconnect();
  client.setCallback(mycallback);
}

void handleMQTT() {
  if (!client.connected()) {
    reconnect();
  }
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
        return menuQuizzOption[4];
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
        Serial.println("Restart Quizz");
        return "EXIT";
      }
    } 
    if (MQTTTopic == "NotABomb/Key/Command" ){
      Serial.println("checkMQTTQuizz Command");
      if (MQTTPayload ==  "EXIT") {
        Serial.println("Restart Quizz");
        return "EXIT";
      }
    }
    if (MQTTTopic == "NotABomb/Challenge" ){
      Serial.println("checkMQTTQuizz NotABomb/Challenge");
      return MQTTPayload;
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
    message.toCharArray(msg, message.length());
    topic.toCharArray(topicChar, topic.length());
    client.publish(topicChar, msg);
    done = true;
  }
  return done;
}

boolean sendMQTT(String topic, String message) {
  boolean done = false;
  if (!client.connected()) {
    reconnect();
  }
  if (client.connected()) {
    message.toCharArray(msg, message.length());
    topic.toCharArray(topicChar, topic.length());
    client.publish(topicChar, msg);
    done = true;
  }
  return done;
}

#endif // MQTT_H