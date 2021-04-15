#include <CMDWifi.h>
#include "GY521.h"

CMDWifi wifi;
GY521 gyro(0x68);

// Pins
const int piezoPin = 3;

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
    Serial.begin(9600);
    wifi.connect(ssid, pass, server, port);

    Wire.begin();
    delay(100);
//    while (gyro.wakeup() == false){
//        Serial.print(millis());
//        Serial.println("\tCould not connect to GY521");
//        delay(1000);
//    }
    gyro.setAccelSensitivity(0);  // 2g
    gyro.setGyroSensitivity(0);   // 250 degrees/s

    gyro.setThrottle();
    gyro.gxe = 1.211;
//    gyro.gye = -1.700;
    gyro.gze = -0.249;
}

void loop(){
    Serial.print(".");
    //int websiteDeviceState = wifi.get(apiKey2);
    int websiteDeviceState = 1;
//    if (!timerFirstLoop) {
//        playSound("startup");
//        timerFirstLoop = true;
//    }

    gyro.read();
//    int x = gyro.getGyroX();
//    int y = gyro.getGyroY();
//    int z = gyro.getGyroZ();
    int x = gyro.getGyroX();
    // int x = 0;
    // int y = gyro.getGyroY();
    int y = 0;
    int z = gyro.getGyroZ();
    //Serial.println(z);
    if (websiteDeviceState >= 1) {
        if (x != 0 || y != 0 || z != 0) {
            Serial.println("yes");
            if (pickedUpCounter >= pickedUpCounterMax) {
                Serial.println("device picked up");
                playSound("alert");
                wifi.send(apiKey1, 1);
            } else { pickedUpCounter++; }
        } else if (x == 0 && y == 0 && z == 0 && pickedUpCounter != 0) {
            wifi.send(apiKey1, 0);
            pickedUpCounter = 0;
        }
    } else if (websiteDeviceState == 0) {
        timerFirstLoop = false;
        playSound("finished");
    }
    delay(loopFrequency);
}

void playSound(String soundName){
    if (soundName == "alert") {
        for(int i = 0; i < 30; i++) {
            tone(piezoPin, 10000, 100);
            delay(100);
            tone(piezoPin, 15000, 200);
            delay(100);
            tone(piezoPin, 30000, 200);
            delay(100);
        }
    } else { Serial.println("fnc playSound: Sound name not recognized"); }
}
