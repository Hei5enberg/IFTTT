#include "GY521.h"

GY521 gyro(0x68);

const int piezoPin = 8;

const int loopFrequency = 500;
const int allowedPickupTime = 10; // Seconds
const int pickedUpCounterMax = (allowedPickupTime * 1000) / loopFrequency;

int pickedUpCounter = 0;

void setup() {
    Serial.begin(9600);
    Wire.begin();
    delay(100);
    while (gyro.wakeup() == false)
  {
    Serial.print(millis());
    Serial.println("\tCould not connect to GY521");
    delay(1000);
  }
  gyro.setAccelSensitivity(0);  // 2g
  gyro.setGyroSensitivity(0);   // 250 degrees/s

  gyro.setThrottle();
  Serial.println("Starting...");
  gyro.gxe = 1.211;
  gyro.gye = -1.700;
  gyro.gze = -0.249;
}

void loop() {
    if (checkGyro()) {
        playSound("alert");
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
    }
}



