#include <Wire.h>
#include "Player.h"
#include "Ball.h"
#include "Display.h"

class Pong {
  private:
    Display display;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    
    Ball ball;
    Player p1;
    Player p2;
  
  public:
    Pong(int SCREEN_WIDTH, int SCREEN_HEIGHT, int SCREEN_ADDRESS, int OLED_RESET, int p1SWup, int p1SWdown, int p2SWup, int p2SWdown) 
      : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, SCREEN_ADDRESS), 
        ball(30, 30, 2, SCREEN_WIDTH, SCREEN_HEIGHT), 
        p1(5, 15, 3, 15, SCREEN_WIDTH, SCREEN_HEIGHT, p1SWup, p1SWdown), 
        p2(120, 15, 3, 15, SCREEN_WIDTH, SCREEN_HEIGHT, p2SWup, p2SWdown) {
        this->SCREEN_WIDTH = SCREEN_WIDTH;
        this->SCREEN_HEIGHT = SCREEN_HEIGHT;
    }
    
    void setup() {
      display.begin();
      p1.setup();
      p2.setup();
    }


    void update() {
      display.clear();
      
      p1.update();
      p2.update();

      ball.checkCollision(p1, p2);
      ball.move();

      checkScore();
      draw();
      display.update();

    }

    void draw() {
      display.drawBall(ball);
      display.drawPlayer(p1);
      display.drawPlayer(p2);
      display.drawScore(p1, p2);
    }

    void checkScore() {
      if(ball.getX() >= 128 - ball.getRadius()) {
        ball.reset(30, 30);
        p1.addScore(1);
      } else if(ball.getX() <= 0 + ball.getRadius()) {
        ball.reset(30, 30);
        p2.addScore(1);
      }
    }

  // Get/Set - Methoden
  int getWidth() {return this->SCREEN_WIDTH;}
  int getHeight() {return this->SCREEN_HEIGHT;}

};