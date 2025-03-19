#include "InputHandler.h"
class Player {
  private:
    int x, y;
    int width, height;
    int speed = 2;
    InputHandler ih;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    int Score = NULL;
  
  public:
    Player(int x, int y, int w, int h, int SCREEN_WIDTH, int SCREEN_HEIGHT, int SWup, int SWdown) 
      : ih(SWup, SWdown) {
      this->x = x;
      this->y = y;
      this->width = w;
      this->height = h;
      this->SCREEN_WIDTH = SCREEN_WIDTH;
      this->SCREEN_HEIGHT = SCREEN_HEIGHT;
    }
    void setup() {
      ih.setup();
    }

    void moveUp() {
      if(this->y > NULL) y-=speed;
      else return;
    }

    void moveDown() {
      if(this->y < this->SCREEN_HEIGHT - this->height) y+=speed;
    }

    void update() {
      if(ih.SWup()) moveUp();
      if(ih.SWdwon()) moveDown();
    }

  public:
    int getX() {return this->x;}
    int getY() {return this->y;}

    int getWidth() {return this->width;}
    int getHeight() {return this->height;}

    int getScore() {return this->Score;}
    
    int setX(int x) {this->x = x;}
    int setY(int y) {this->y = y;}

    int setWidth(int width) {this->width = width;}
    int setHeight(int height) {this->height = height;}

    int setScore(int Score) {this->Score = Score;}
    int addScore(int i) {this->Score+=i;}
    
};
