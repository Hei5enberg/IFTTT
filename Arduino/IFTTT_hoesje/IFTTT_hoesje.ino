#include <CMDWifi.h>

CMDWifi wifi;

// WiFi settings
char ssid[] = "AvansWlan"; 
char pass[] = ""; 
char server[] = "cmd.camp"; 
int port = 12345; 
char apiKey1[] = ;
char apiKey2[] = ; 

// Timer variables
int timerStartTime;
int alarmDelay = 120000; // 2 minutes
bool timerRunning = false;

void setup() {
    pinMode(3, OUTPUT);

    Serial.begin(9600);

    Serial.println("Connecting to WiFi...");
    wifi.connect(ssid, pass, server, port);
}

void loop(){
    int websiteDeviceState = receiveData(apiKey2);
    if (websiteDeviceState == 1) {
        Serial.println(" ");
        Serial.println("DEVICE NOT IN USE");
        deviceActive();
    } else {
        Serial.println(" ");
        Serial.println("DEVICE NOT IN USE");
    }
    delay(50);
}

bool checkGyro() {
    return false;
}

void deviceActive() {
    bool devicePickedUp = checkGyro();
    
    if (devicePickedUp) {
        Serial.println("Device picked up");
        if (!timerRunning) {
            Serial.println("Starting timer...");
            timerStartTime = millis();
            timerRunning = true;
        } else if (timerRunning && (timerStartTime - millis()) >= alarmDelay) {
            Serial.print("Device still active after: ");
            Serial.print(alarmDelay / 1000);
            Serial.println("s");
            timerRunning = false;
            Serial.println("Sending alert to server...");
            sendData(apiKey1, 1);
        }
    }
}

void sendData(char apiKey, int value) {
    Serial.println("Sending data...");
    int response = wifi.send(apiKey, value);
    if (response != -1){
        Serial.print("Value send to: ");
        Serial.println(apiKey);
        Serial.print(">");
        Serial.println(value);
    } else { Serial.println("ERROR: couldn't send value"); }
}

int receiveData(char apiKey) {
    Serial.println("Reading data...");
    int response = wifi.get(apiKey);

    if (response >= 0 ) {
        Serial.print("Vale read from: ");
        Serial.println(response);
        Serial.print(">");
        Serial.println(response);
    } else { 
        Serial.println("ERROR: couldn't read data"); 
        response = -1;    
    }
    return response;
}