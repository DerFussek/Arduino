#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class Display {
  private:
    Adafruit_SSD1306 display;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    int OLED_RESET;
    int SCREEN_ADDRESS;

  public:
    Display(int SCREEN_WIDTH, int SCREEN_HEIGHT, TwoWire *wire, int OLED_RESET, int SCREEN_ADDRESS) 
      : display(SCREEN_WIDTH, SCREEN_HEIGHT, wire, OLED_RESET) {
        this->SCREEN_WIDTH = SCREEN_WIDTH;
        this->SCREEN_HEIGHT = SCREEN_HEIGHT;
        this->OLED_RESET = OLED_RESET;
        this->SCREEN_ADDRESS = SCREEN_ADDRESS;
    }

    void begin() {
      if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("SSD1306 Fehler!");
        while(true); // Endlosschleife im Fehlerfall
      }
        display.setTextColor(1);
        display.clearDisplay();
        display.display();
    }

    void drawText(int x, int y, int scale, String txt) {
      display.setTextSize(scale);
      display.setCursor(x, y);
      display.println(txt);
    }

    void drawPlayer(Player p) {
      display.fillRect(p.getX(), p.getY(), p.getWidth(), p.getHeight(), 1);
    }

    void drawBall(Ball b) {
      display.fillCircle(b.getX(), b.getY(), b.getRadius(), 1);
    }

    void drawScore(Player p1, Player p2) {
      drawText(0, 0, 1, String(p1.getScore()));
      
      if(p2.getScore() < 10) drawText(120, 0, 1, String(p2.getScore()));
      else drawText(115, 0, 1, String(p2.getScore()));
     
    }

    void clear() {
      display.clearDisplay();
    }

    void update() {
      display.display();
    }

};