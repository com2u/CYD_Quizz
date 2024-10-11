#include <WiFi.h>
#include "globals.h" // For PROJECT_NAME, ssid, and password arrays
#include <Credentials.h>



//generate unique name from MAC addr
String macToStr(const uint8_t* mac){
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5){
      result += ':';
    }
  }
  return result;
}


String getFingerprint(const uint8_t* mac){
  String result;
  for (int i = 4; i < 6; ++i) {
    result += String(mac[i], 16);
  }
  return result;
}

boolean connectWIFI(String ssid,String password, int retries ){
  Serial.print("Connecting ");
  Serial.println(ssid);
  delay(100);
  WiFi.begin(ssid, password);
  String clientName =  PROJECT_NAME;
  uint8_t mac[6];
  WiFi.macAddress(mac);
  clientName += getFingerprint(mac);
  char host[clientName.length()];
  clientName.toCharArray(host, clientName.length());
  for (int retry = 0; retry < retries; retry++) {
    if (WiFi.status() == WL_CONNECTED) {
      WiFi.hostname(host);
      break;
    } else {
      Serial.print(".");
      delay(1000);
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected as ");
    Serial.println(host);
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  }
  return false;
}


void initWIFI() {

  
  //connect to WiFi
  int networkNo = 0;
  int ssidSize = 0;
  int retries = 12;
  //attempt to connect to the wifi if connection is lost
  if (WiFi.status() != WL_CONNECTED) {

    // Try several WIFI Networks
    //for(networkNo = 0; networkNo < 10; networkNo++) {
    while (ssidSize < sizeof(ssid) ) {
      ssidSize += sizeof(ssid[networkNo]);
      if (connectWIFI(ssid[networkNo], password[networkNo],retries)){
        break;
      }
      networkNo++;
    }
  }
  // Create Acces Point when no WIFI found
  /*
  if (retry == retries) {
    Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());
  }
  */


}


void scanWIFINetwork() {
  Serial.println((String) "WIFI scan start "+millis());

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println((String) "WIFI scan done "+millis());
  if (n == 0) {
      Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      menuWIFI[i] = WiFi.SSID(i);
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      int ssidSize = 0;
      int networkNo = 0;
      while (ssidSize < sizeof(ssid) ) {
        ssidSize += sizeof(ssid[networkNo]);
        if (WiFi.SSID(i) == ssid[networkNo]){
            Serial.print(" Home Network found: ");
            if (connectWIFI(ssid[networkNo], password[networkNo],10)){
              return;
            }
        }
        networkNo++;
      }
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(500);
}

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    WiFi.disconnect();
    initWIFI();
  }
}
