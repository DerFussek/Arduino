
#include "Pong.h"
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Pong pong(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_ADDRESS, OLED_RESET, 3, 4, 5, 6);

void setup() {
  Serial.begin(9600);
  pong.setup();
}

void loop() {
  pong.update();

}
