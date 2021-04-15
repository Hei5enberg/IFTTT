#include <WiFi.h>
#include <HTTPClient.h>

// const char* ssid = "";
// const char* password = "";

// String apiKey1 = "";
// String apiKey2 = "";


void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
    if(WiFi.status() == WL_CONNECTED){
        //String serverPath = "http://cmd.camp:12345/get/" + apiKey1;
        for(int i = 0; i < 30; i++) {
            //String serverPath = "http://cmd.camp:12345/send/" + apiKey1 + "/1234";
            String serverPath = "http://cmd.camp:12345/send/" + apiKey1 + "/" + i;
            //String serverPath = "http://cmd.camp:12345/get/" + apiKey1;
            Serial.println(serverPath);
            sendAndReceive(serverPath.c_str());
            //httpPOSTRequest(serverPath.c_str(), i);
            delay(1000);
        }
        Serial.println("Waiting 10 seconds for second run");
        delay(10000);
    } else { Serial.println("WiFi Disconnected"); }
}

String sendAndReceive(const char* serverName) {
  HTTPClient http; 
  http.begin(serverName);
  
  int httpResponseCode = http.GET();
  
  String payload;
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    Serial.print("Payload: ");
    Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}