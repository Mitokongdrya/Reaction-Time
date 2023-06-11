#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>
const uint8_t spPAUSE1[] PROGMEM = { 0x08, 0x14, 0xC1, 0xDD, 0x45, 0x64, 0x03, 0x00, 0xFC, 0x4A, 0x56, 0x26, 0x3A, 0x06, 0x0A };

const uint8_t spPLUS[] PROGMEM = { 0x02, 0xF0, 0xD6, 0xCC, 0x01, 0x51, 0x8B, 0x8D, 0xA2, 0x29, 0x65, 0x4B, 0x5F, 0x3D, 0x8B, 0x51, 0x34, 0xB3, 0x99, 0x2E, 0x0B, 0x4F, 0xD9, 0xF4, 0x66, 0x84, 0xB6, 0x3C, 0x75, 0x35, 0x93, 0x91, 0x92, 0xE9, 0x54, 0xCD, 0x4C, 0x65, 0x72, 0xAB, 0x55, 0x95, 0xE0, 0x15, 0x1C, 0xB5, 0x5B, 0x3D, 0x49, 0x86, 0x9B, 0x79, 0x52, 0xC0, 0xC5, 0x1C, 0x1A, 0xD0, 0x80, 0x06, 0x04, 0xF0, 0x04, 0xF9, 0xFF, 0x15, 0x39, 0x49, 0x79, 0x05 };

const uint8_t spONE[] PROGMEM = { 0xA6, 0x90, 0x76, 0xCC, 0x43, 0x6B, 0xB9, 0x42, 0xD8, 0x35, 0x77, 0xA9, 0x19, 0x72, 0xE5, 0x46, 0xD2, 0x62, 0x56, 0xCB, 0xAC, 0x9F, 0x68, 0xE1, 0x4E, 0x23, 0x09, 0x71, 0x23, 0x55, 0x3A, 0x9E, 0xB8, 0xC4, 0x4B, 0x77, 0x6D, 0x74, 0xE3, 0x93, 0x54, 0x7B, 0xE5, 0x6E, 0x2D, 0x4F, 0xD6, 0xEC, 0x56, 0x84, 0xB5, 0x3A, 0x45, 0x0D, 0x5D, 0xA1, 0xD6, 0xA8, 0xF4, 0xC5, 0x58, 0x84, 0x78, 0xEC, 0x34, 0x64, 0x6D, 0x16, 0xEA, 0x8D, 0xC2, 0x90, 0x95, 0x66, 0xB1, 0x25, 0x0E, 0x43, 0x92, 0x1A, 0x43, 0x9E, 0x26, 0x0C, 0x49, 0x49, 0xB4, 0x78, 0xEA, 0x34, 0x16, 0x1B, 0xA6, 0xEA, 0xB1, 0xC9, 0x24, 0x82, 0xBB, 0x88, 0xC8, 0x06, 0x00, 0xF8, 0x26 };

float midi[127];
int A_four = 440;
int nextLevel[3][2]{
  {64, 160},
  {64, 80},
  {64, 320},
};
int tryAgain[2][2]{
  {62, 100},
  {63, 100},
};


volatile int val;
volatile int spinSpeed = 100;


volatile bool instStop = 0;
const int leftButt = 4;
const int rightButt = 5;
volatile bool leftFlag = 0;
volatile bool rightFlag = 0;



void setup() {

  CircuitPlayground.begin();
  Serial.begin(9600);
  generateMIDI();
  attachInterrupt(leftButt, gameButton, RISING);
  attachInterrupt(rightButt, controlButton, RISING);
}

void loop() {
  CircuitPlayground.speaker.set(250);
  for (int i = 0; i < 10 && !leftFlag; i++) {
    CircuitPlayground.setBrightness(5);
    CircuitPlayground.setPixelColor(i, 255, 65, 0);
    CircuitPlayground.setPixelColor(i - 1, 255, 65, 0);
    CircuitPlayground.setPixelColor(i + 1, 255, 65, 0);
    delay(spinSpeed);
    CircuitPlayground.setPixelColor(i, 0, 0, 0);
    CircuitPlayground.setPixelColor(i + 1, 0, 0, 0);
    CircuitPlayground.setPixelColor(i - 1, 0, 0, 0);
    delay(1);
    int val = i;
    if (leftFlag) {
      if (val == 7) {
        for(int i = 0; i < sizeof(nextLevel) / sizeof(nextLevel[0]); i++){
      CircuitPlayground.playTone(midi[nextLevel[i][0]], nextLevel[i][1]);
      blink();
      delay(1);
    }

        // blink();
        // blink();
        // blink();
        CircuitPlayground.speaker.say(spPLUS);
        CircuitPlayground.speaker.say(spONE);
        CircuitPlayground.speaker.end();

        spinSpeed -= 10;
        if(spinSpeed == 90){
          winLights();
        }
      } else
        Serial.println(val);
      digitalWrite(val, HIGH);
      CircuitPlayground.setPixelColor(val, 255, 65, 0);
      CircuitPlayground.setPixelColor(val - 1, 0, 0, 0);
      CircuitPlayground.setPixelColor(val + 1, 0, 0, 0);
      delay(3000);

      leftFlag = 0;
    }
  }

  if (rightFlag) {
    spinSpeed = 100;
    rightFlag = 0;
  }
}