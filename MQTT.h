#include <PubSubClient.h>
#include <ArduinoJson.h>


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



void mycallback(char* topic, byte* payload, unsigned int length) {
  //int outputNo = -1;
  //Serial.println("Callback triggered"); 
  uint8_t value;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  //Serial.println(payload);
  String command = "";
  Serial.print("Payload [");
  
  for (int i = 0; i < length; i++) {
    command += (char)payload[i];
    Serial.print((char)payload[i]);
  }
  Serial.print("]");
  
  Serial.print(command);
  if (command.equals("0")) {
    value = LOW;
  } else if (command.equals("1")) {
    value = HIGH;
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


