#include <WiFi.h>
#include <HTTPClient.h>
#include "GY521.h"

// REMOVE THESE BEFORE PUSHING


// Pins
const int piezoPin = 15;

// I2c adress GY521
GY521 gyro(0x68);

// Loop frequency, maximum allowed time before alarm and counter
const int loopFrequency = 500;
const int allowedPickupTime = 5;
const int pickedUpCounterMax = (allowedPickupTime * 1000) / loopFrequency;
int pickedUpCounter = 0;
int setDownCounter = 0;

// Web timer variables
bool timerFirstLoop = false;

// Variables for tone()
int freq = 2000;
int channel = 0;
int resolution = 8;

void setup() {
    // Serial setup
    Serial.begin(115200);
    for (int i = 0; i < 50; i++) {
      if (i != 50) {
        Serial.print("-");
      } else { Serial.println("-"); }
    }
    Serial.println("");
    Serial.println("ESP: Started serial monitor");

    // Pin setup
    ledcSetup(channel, freq, resolution);
    ledcAttachPin(12, channel);

    // Gyro setup
	Wire.begin();
	delay(100);
	while (gyro.wakeup() == false) {
		Serial.print(millis());
		Serial.println("\tCould not connect to GY521");
		delay(1000);
	}
    gyro.setGyroSensitivity(2);

    gyro.setThrottle();
    Serial.println("ESP: starting gyro...");
    gyro.gxe = 1.321;
    gyro.gye = -1.812;
    gyro.gze = -0.220;

    // Wifi Setup
    WiFi.begin(ssid, password);

    Serial.println("ESP: WiFi connecting");
    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.print("ESP: Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
}

void loop() {
    if(WiFi.status() == WL_CONNECTED){
		// if (!timerFirstLoop) {
		// 	playSound("startup");
		// 	timerFirstLoop = true;
		// }

		gyro.read();
		int x = gyro.getGyroX();
		int y = gyro.getGyroY();
		int z = gyro.getGyroZ();

		Serial.print("Gyro: ");
		Serial.print("x: ");
		Serial.print(x);
		Serial.print(", y: ");
		Serial.print(y);
		Serial.print(", z: ");
		Serial.println(z);

    if (x != 0 || y != 0 || z != 0 ){
		int websiteTimeLeft = (receiveRequest(apiKey2)).toInt();
		if (websiteTimeLeft > 0) {
			if (pickedUpCounter > pickedUpCounterMax) {
				Serial.print("ESP: user has been using device for ");
		        Serial.print(allowedPickupTime);
		        Serial.println(" seconds");
				writeRequest(apiKey1, 1);
				playSound("alarm");
			} else { pickedUpCounter++; }
		} else if (websiteTimeLeft == 0) {
			Serial.print("ESP: user picked up device but timer is finished");
			timerFirstLoop = false;
			playSound("finished");
			// Writing -1 to website key to ensure sound won't run on a loop
			writeRequest(apiKey2, -1);
		}
    } else if (x == 0 && y == 0 && z == 0) {
		if (setDownCounter < 10 && pickedUpCounter > 0) {
			setDownCounter++;
		} else if (setDownCounter >= 10) {
			Serial.println("ESP: resetting pickup counter");
			pickedUpCounter = 0;
			writeRequest(apiKey1, 0);
		}
    }

		// if (websiteTimeLeft >= 1) {
		// 	Serial.print("Website: time left ");
		// 	Serial.print(websiteTimeLeft);
		// 	Serial.println("s");

    //     if (x != 0 || y != 0 || z != 0) {
    //       if (pickedUpCounter >= pickedUpCounterMax) {
    //         Serial.print("ESP: user has been using device for ");
    //         Serial.print(allowedPickupTime);
    //         Serial.println(" seconds");
    //         writeRequest(apiKey1, 1);
    //       } else { pickedUpCounter++; }
    //     } else if (x == 0 && y == 0 && z == 0) {
    //         Serial.println("ESP: not picked up");
    //         pickedUpCounter = 0;
    //         writeRequest(apiKey1, 0);
    //     }
        
    //   } else if (websiteTimeLeft == 0) {
    //       timerFirstLoop = false;
    //       playSound("finished");
    //       // Writing a -1 so the finished sound won't be repeated
    //       writeRequest(apiKey2, -1);
    //   }
      delay(loopFrequency);
       
    } else { Serial.println("ESP: WiFi Disconnected"); }
}

String receiveRequest(String apiKey) {
	String httpString = "http://cmd.camp:12345/get/" + apiKey;
	String requestReturn = httpRequest(httpString);
	return requestReturn;
}

void writeRequest(String apiKey, int value) {
	String httpString = "http://cmd.camp:12345/send/" + apiKey + "/" + value;
	httpRequest(httpString);
}

String httpRequest(String serverName) {
	Serial.println("ESP: sending a GET request...");

	HTTPClient http; 
	http.begin(serverName.c_str());

	int httpResponseCode = http.GET();

	String payload = "";
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

	http.end();

	return payload;
}

void playSound(String soundName){
    Serial.print("ESP: Playing sound \"");
    Serial.print(soundName);
    Serial.println("\"");
    if (soundName == "alarm") {
        for(int i = 0; i < 1; i++) {
            ESPTone(3333, 1750);
            delay(100);
            ESPTone(6666, 500);
            delay(100);
            ESPTone(9999, 1500);
            delay(100);
        }
    } else { Serial.println("ESP: Sound name not recognized"); }
}

void ESPTone(int freq, int duration) {
    ledcWriteTone(channel, freq);
    ledcWrite(channel, 255);
    delay(duration);
    ledcWrite(channel, 0);
}