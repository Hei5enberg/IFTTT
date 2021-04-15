#include <CMDWifi.h>
#include "GY521.h"

CMDWifi wifi;
GY521 gyro(0x68);

// Pins
const int piezoPin = 2;

// Loop frequency and maximum allowed time
const int loopFrequency = 500; // Milliseconds
const int allowedPickupTime = 5; // Seconds
const int pickedUpCounterMax = (allowedPickupTime * 1000) / loopFrequency;
int pickedUpCounter = 0;

// WiFi settings
char ssid[] = ""; 
char pass[] = ""; 
char server[] = "cmd.camp"; 
int port = 12345; 
char apiKey1[] = "";
char apiKey2[] = ""; 

// Timer variables
int timerStartTime;
int alarmDelay = 120000; // 2 minutes
bool timerRunning = false;

// Web timer variable
bool timerFirstLoop = false;

void setup() {
    pinMode(3, OUTPUT);

    Serial.begin(9600);

    Serial.println("Connecting to WiFi...");
    wifi.connect(ssid, pass, server, port);
}

void loop(){
    int websiteDeviceState = receiveData(apiKey2);
    if (!timerFirstLoop) {
        playSound("startup");
        timerFirstLoop = true;
    }

    if (websiteDeviceState >= 1) {
        Serial.println(" ");
        Serial.println("TIMER RUNNING ON WEBAPP");
        if (checkGyro()) {
            playSound("alert");
        }
    } else if (websiteDeviceState == 0) {
        timerFirstLoop = false;
        playSound("finished");
    }
    delay(loopFrequency);
}

bool checkGyro() {
    gyro.read();
    int x = gyro.getGyroX();
    int y = gyro.getGyroY();
    int z = gyro.getGyroZ();

    Serial.print("x: ");
    Serial.println(x);
    Serial.print("y: ");
    Serial.println(y);
    Serial.print("z: ");
    Serial.println(z);

    if (x != 0 || y != 0 || z != 0) {
        if (pickedUpCounter >= pickedUpCounterMax) {
            Serial.print("Device: have been picked up for ");
            Serial.print(allowedPickupTime);
            Serial.println(" seconds");
            return true;
        } else { pickedUpCounter++; }
    } else if (x == 0 && y == 0 && z == 0) {
        Serial.println("Device: not picked up");
        pickedUpCounter = 0;
        return false;
    }
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

void playSound(String soundName){
    Serial.print("fnc playSound: Playing sound \"");
    Serial.print(soundName);
    Serial.println("\"");
    if (soundName == "alert") {
        for(int i = 0; i < 5; i++) {
            tone(piezoPin, 10000, 100);
            delay(100);
            tone(piezoPin, 15000, 200);
            delay(100);
            tone(piezoPin, 30000, 200);
            delay(100);
        }
    } else { Serial.println("fnc playSound: Sound name not recognized"); }
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