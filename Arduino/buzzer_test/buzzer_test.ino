// Variables for tone()
int baseFreq = 2000;
int channel = 0;
int resolution = 8;

void setup() {
    // Serial setup
    Serial.begin(115200);

    // Pin setup
    ledcSetup(channel, baseFreq, resolution);
    ledcAttachPin(12, channel);
}

void loop() {
    // ledcWriteTone(channel, 2000);
    // for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle=dutyCycle+10){
    //     Serial.println(dutyCycle);
    //     ledcWrite(channel, dutyCycle);
    //     delay(1000);
    // }

    ESPTone(3333, 1750);
    delay(100);
    ESPTone(6666, 500);
    delay(100);
    ESPTone(9999, 1500);
    delay(100);
}

void ESPTone(int freq, int duration) {
    ledcWriteTone(channel, freq);
    ledcWrite(channel, 255);
    delay(duration);
    ledcWrite(channel, 0);
}