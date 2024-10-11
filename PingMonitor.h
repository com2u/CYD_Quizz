#include <ESP32Ping.h>
#include <HTTPClient.h>
TaskHandle_t Core0Task;
TaskHandle_t Core1Task;

const char* hosts[] = {
  "192.168.0.1", 
  "192.168.0.3",
  "192.168.0.168", 
  "nuc.fritz.box", 
  "SVIMServer",
  "Proxmox3",
  "Proxmox4",
  "192.168.0.6",
  "Controller",
  "AIServer",
  "SightMap",
  "octopi",
  "wled-PictureFrame",
  "116.203.60.216"
};

const char* urls[] = {
  "https://com2u.de/",
  //"http://com2u.selfhost.eu/",
  "https://seidenader.ejectx.de/login",
  //"http://sightmap.com2u.selfhost.eu/",
  "https://ai-server.com2u.selfhost.eu/",
  "https://test.ejectx.de/login",
  //"http://100.87.235.66:3078",
  //"http://mischen.jetzt/",
  "https://www.heise.de/",
  "https://www.golem.de/",
  "https://www.google.com/",
  "https://www.spiegel.de/"
  
};

const int numHosts = sizeof(hosts) / sizeof(hosts[0]);
const int numUrls = sizeof(urls) / sizeof(urls[0]);
String hostStatus[numHosts];
int urlStatusCount[numUrls];
int hostStatusCount[numHosts];
String urlStatus[numUrls];

String statusValue="";

int lineCount = 0;
const int maxLines = 20;

// Function to send the second POST request (set segment name to "Com2u!")
void sendWLEDPostRequest2(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Specify the WLED URL
    http.begin(wled_ip);

    // Set headers
    http.addHeader("Content-Type", "application/json");

    // Create JSON payload
    String jsonPayload = "{\"seg\": [{\"n\": \""+message+"\"}]}";

    // Send POST request
    int httpResponseCode = http.POST(jsonPayload);

    // Check the responsea
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response for Post 2: " + response);
    } else {
      Serial.println("Error on sending POST 2: " + String(httpResponseCode));
    }

    // Close connection
    http.end();
  }
}

// Function to send the first POST request (turn on with scrolling text "Hello World")
void sendWLEDPostRequest1() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    // Specify the WLED URL
    http.begin(wled_ip);

    // Set headers
    http.addHeader("Content-Type", "application/json");

    // Create JSON payload
    String jsonPayload = "{\"on\":true, \"v\":true, \"seg\":[{\"fx\":122, \"ps\":\"Hello World\"}]}";

    // Send POST request
    int httpResponseCode = http.POST(jsonPayload);

    // Check the response
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response for Post 1: " + response);
    } else {
      Serial.println("Error on sending POST 1: " + String(httpResponseCode));
    }

    // Close connection
    http.end();
  }
}


// Function to send the first POST request (turn on with scrolling text "Hello World")
void sendWLEDPostRequest1b() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    // Specify the WLED URL
    http.begin(wled_ip);

    // Set headers
    http.addHeader("Content-Type", "application/json");

    // Create JSON payload
    String jsonPayload = "{\"seg\":{\"id\":0,\"col\":[[255,160,0],[0,0,0],[0,0,0]]}}";
    // curl -X POST http://192.168.0.90/json/state -H "Content-Type: application/json" -d "{\"seg\":{\"id\":0,\"col\":[[255,160,0],[70,0,70],[0,0,0]]}}"

    // Send POST request
    int httpResponseCode = http.POST(jsonPayload);

    // Check the response
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response for Post 1b: " + response);
    } else {
      Serial.println("Error on sending POST 1b: " + String(httpResponseCode));
    }

    // Close connection
    http.end();
  }
}

void WLEDMessage(String text){
      sendWLEDPostRequest1();  // First POST request
      delay(1000);         // Delay between requests
      sendWLEDPostRequest1b();  // First POST request
      delay(1000);         // Delay between requests
      sendWLEDPostRequest2(text);  // Second POST request
  
}


void printLine(String text,String status,int  responsecount) {
  if (lineCount >= maxLines) {
    CYD_TFT_Top();
    lineCount = 0;
  }
  uint16_t color = responsecount == 0 ? TFT_GREEN : TFT_RED;
  text = String(responsecount)+" "+text;
  CYD_TFT_print(text, color, TFT_WHITE);
  Serial.print(text);
  Serial.print("  ");
  Serial.println(status);
  lineCount++;
}





void pingHosts() {
  CYD_LED_Green();
  for (int i = 0; i < numHosts; i++) {
    bool success = Ping.ping(hosts[i]);
    statusValue = success ? "OK" : "FAILED";
    
    if (statusValue != "OK"){
      hostStatusCount[i]++;
      WLEDMessage(hosts[i]);
      CYD_LED_Red();
      delay(1000);
    } else {
      hostStatusCount[i] = 0;
    }
    printLine(String(hosts[i]),statusValue, hostStatusCount[i]);
    if (global_state != 4) { return; }
  }
}

void checkUrls() {
  HTTPClient http;
  http.setTimeout(1000);
  //http.setHttpResponseTimeout(1000);

  for (int i = 0; i < numUrls; i++) {
    //MaxResponseContentBufferSize = 9999999; 
    http.begin(urls[i]);
    int httpCode = http.GET();
    statusValue = (httpCode > 0) ? "OK" : "FAILED";
    if (statusValue != "OK"){
      WLEDMessage(urls[i]);
      urlStatusCount[i]++;
      CYD_LED_Red();
      delay(1000);
    } else {
      Serial.print(httpCode);
      Serial.print("-ErrorCode ");
      urlStatusCount[i] = 0;
    }

    
    printLine(String(urls[i]),statusValue, urlStatusCount[i]);
    
    delay(200);
    

    //http.flush();
    http.end();
    if (global_state != 4) { return; }
    delay(3000);
  }
}


