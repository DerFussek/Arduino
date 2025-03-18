class Ball {
    private:
      int x, y;
      int vx, vy;
      int radius;
      int SCREEN_WIDTH;
      int SCREEN_HEIGHT;
    public:
      Ball(int x, int y, int radius, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
        this->x = x;
        this->y = y;
        this->radius = radius;
        this->vx = 2;
        this->vy = 2;
        this->SCREEN_WIDTH = SCREEN_WIDTH;
        this->SCREEN_HEIGHT = SCREEN_HEIGHT;
      }
  
      void move() {
        x += vx;
        y += vy;
      }
  
      void checkCollision(Player p1, Player p2) {
        // Prüfe Kollision mit den oberen und unteren Spielfeldrändern
        if (y <= 0 - radius || y + radius >= SCREEN_HEIGHT) {
          vy = -vy;
        }
  
        // Prüfe Kollision mit Spieler 1
        if (x - radius <= p1.getX() + p1.getWidth() && y >= p1.getY() && y <= p1.getY() + p1.getHeight()) {
          vx = -vx;
        }
        
        // Prüfe Kollision mit Spieler 2
        if (x + radius >= p2.getX() && y >= p2.getY() && y <= p2.getY() + p2.getHeight()) {
          vx = -vx;
        }
      }
  
      void reset(int x, int y) {
        this->x = x;
        this->y = y;
        this->vx = 2; 
        this->vy = 2;
      }
  
    public:  
      // Get/Set - Methoden
      int getX() {return this->x;}
      int getY() {return this->y;}
  
      int getVX() {return this->vx;}
      int getVY() {return this->vy;}
  
      int getRadius() {return this->radius;}
      
      int setX(int x) {this->x = x;}
      int setY(int y) {this->y = y;}
  
      int setVX(int vx) {this->vx = vx;}
      int setVY(int vy) {this->vy = vy;}
  
      int setRadius(int r) {this->radius = r;}
  
  
  
  };