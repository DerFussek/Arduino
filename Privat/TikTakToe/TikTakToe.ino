#define ArduinoMega

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
  MCUFRIEND_kbv tft;
#include <TouchScreen.h>

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define computerColor GREEN
#define playerColor RED
#define TITLEcolor RAINBOW

const int XP = 6, XM = A2, YP = A1, YM = 7; //ID=0x9341
const int TS_LEFT = 907, TS_RT = 136, TS_TOP = 942, TS_BOT = 139;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

int pixel_x, pixel_y;     //Touch_getXY() updates global vars
bool Touch_getXY(void) {
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    }
    return pressed;
}

bool running = true;
char spaces[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
char player = 'X';
char computer = 'O';
unsigned int winncase;
bool move = 0;  //wenn 0, dann ist spieler am zug, wenn 1 dann computer

#if !defined(computerColor) || !defined(playerColor)
  #error Bitte computer- und playerColor definieren
#endif

void setup(void) {
  Serial.begin(115200);
  Serial.print("Initialisiere TFT Screen...");
  uint16_t ID = tft.readID();
  if(ID == 0xD3D3) ID = 0x9486;  // write-only shield
  tft.begin(ID);
  tft.setRotation(0);  //PORTRAIT
  tft.fillScreen(BLACK);
  delay(500);
  tft.fillScreen(MAGENTA);
  delay(500);
  tft.fillScreen(BLACK);

  drawBoard(spaces);
  while(running) {
    
    drawBoard(spaces);
    rainbow()
    playerMove(spaces, player);
    if(checkWinner(spaces, player, computer) == true)
        {
            Serial.println("hah");
            running = false;
            break;
        } else if(checkTie(spaces)) {
          
          running = false;
          printTie(spaces);
          return;
        }
    drawBoard(spaces);
    rainbow()
    computerMove(spaces, computer);
    if(checkWinner(spaces, player, computer) == true)
        {   
          Serial.println("hah");
            running = false;
            break;
        } else if(checkTie(spaces)) {
          running = false;
          printTie(spaces);
          return;
        }
  }
}

void loop(void) {
 
} 

void rainbow() {
  #ifdef TITLEcolor
    int i = 1;
    if(TITLEcolor == RAINBOW) {
      tft.fillScreen(BLACK);
    tft.setTextColor(CYAN);
    tft.setTextSize(4);
    tft.setCursor(10, 25);
    
     #ifdef TITLEcolor
        do {
          
          switch(i) {
            case 1:
              tft.setTextColor(BLUE);
            break;
            case 2:
              tft.setTextColor(RED);  
            break;
            case 3:
              tft.setTextColor(GREEN);  
            break;
            case 4:
              tft.setTextColor(CYAN);  
            break;
            case 5:
              tft.setTextColor(MAGENTA);  
            break;
            case 6:
              tft.setTextColor(YELLOW);  
            break;
            case 7:
              tft.setTextColor(WHITE);
            break;
            i++;
            if(i > 7) {
              i = 1;
            }
            
            default:
            
              tft.sestTextColor(CYAN);
          }
        } while(TITLEcolor == RAINBOW && i < 8)  
          
        
        }
      #endif
    
    tft.println("TikTakToe");
    tft.drawLine(0, 80, 240, 80, WHITE);
    tft.drawLine(1, 80 , 1, 320, WHITE);
    tft.drawLine(1*80, 80 , 1*80, 320, WHITE);
    tft.drawLine(2*80, 80 , 2*80, 320, WHITE);
    tft.drawLine(239, 80 , 239, 320, WHITE);
    tft.drawLine(0, 2*80, 320, 2*80, WHITE);
    tft.drawLine(0, 3*80, 320, 3*80, WHITE);
    tft.drawLine(0, 4*80-1, 320, 4*80-1, WHITE);
    
    tft.setTextSize(4);
    if(spaces[0] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
      tft.setCursor(0*80+30, 1*80+25);
      tft.println(spaces[0]);
      if(spaces[1] == player) {
        #ifdef playerColor
          if(playerColor == BLUE) {
            tft.setTextColor(BLUE);
          } else if(playerColor == RED) {
            tft.setTextColor(RED);
          } else if(playerColor == GREEN) {
            tft.setTextColor(GREEN);
          } else if(playerColor == CYAN) {
            tft.setTextColor(CYAN);
          } else if(playerColor == MAGENTA) {
            tft.setTextColor(MAGENTA);
          } else if(playerColor == YELLOW) {
            tft.setTextColor(YELLOW);
          } else if(playerColor == WHITE) {
            tft.setTextColor(WHITE);
          }    
        #endif
      } else {
          #ifdef computerColor 
            if(computerColor == BLUE) {
            tft.setTextColor(BLUE);
          } else if(computerColor == RED) {
            tft.setTextColor(RED);
          } else if(computerColor == GREEN) {
            tft.setTextColor(GREEN);
          } else if(computerColor == CYAN) {
            tft.setTextColor(CYAN);
          } else if(computerColor == MAGENTA) {
            tft.setTextColor(MAGENTA);
          } else if(computerColor == YELLOW) {
            tft.setTextColor(YELLOW);
          } else if(computerColor == WHITE) {
            tft.setTextColor(WHITE);
          }    
          #endif
    }
      tft.setCursor(1*80+30, 1*80+25);
      tft.println(spaces[1]);
    if(spaces[2] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
      tft.setCursor(2*80+30, 1*80+25);
      tft.println(spaces[2]);
  
    //
  
      if(spaces[3] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
      tft.setCursor(0*80+30, 2*80+25);
      tft.println(spaces[3]);
      if(spaces[4] == player) {
        #ifdef playerColor
          if(playerColor == BLUE) {
            tft.setTextColor(BLUE);
          } else if(playerColor == RED) {
            tft.setTextColor(RED);
          } else if(playerColor == GREEN) {
            tft.setTextColor(GREEN);
          } else if(playerColor == CYAN) {
            tft.setTextColor(CYAN);
          } else if(playerColor == MAGENTA) {
            tft.setTextColor(MAGENTA);
          } else if(playerColor == YELLOW) {
            tft.setTextColor(YELLOW);
          } else if(playerColor == WHITE) {
            tft.setTextColor(WHITE);
          }    
        #endif
      } else {
          #ifdef computerColor 
            if(computerColor == BLUE) {
            tft.setTextColor(BLUE);
          } else if(computerColor == RED) {
            tft.setTextColor(RED);
          } else if(computerColor == GREEN) {
            tft.setTextColor(GREEN);
          } else if(computerColor == CYAN) {
            tft.setTextColor(CYAN);
          } else if(computerColor == MAGENTA) {
            tft.setTextColor(MAGENTA);
          } else if(computerColor == YELLOW) {
            tft.setTextColor(YELLOW);
          } else if(computerColor == WHITE) {
            tft.setTextColor(WHITE);
          }    
          #endif
      }
      tft.setCursor(1*80+30, 2*80+25);
      tft.println(spaces[4]);
      if(spaces[5] == player) {
        #ifdef playerColor
          if(playerColor == BLUE) {
            tft.setTextColor(BLUE);
          } else if(playerColor == RED) {
            tft.setTextColor(RED);
          } else if(playerColor == GREEN) {
            tft.setTextColor(GREEN);
          } else if(playerColor == CYAN) {
            tft.setTextColor(CYAN);
          } else if(playerColor == MAGENTA) {
            tft.setTextColor(MAGENTA);
          } else if(playerColor == YELLOW) {
            tft.setTextColor(YELLOW);
          } else if(playerColor == WHITE) {
            tft.setTextColor(WHITE);
          }    
        #endif
      } else {
          #ifdef computerColor 
            if(computerColor == BLUE) {
            tft.setTextColor(BLUE);
          } else if(computerColor == RED) {
            tft.setTextColor(RED);
          } else if(computerColor == GREEN) {
            tft.setTextColor(GREEN);
          } else if(computerColor == CYAN) {
            tft.setTextColor(CYAN);
          } else if(computerColor == MAGENTA) {
            tft.setTextColor(MAGENTA);
          } else if(computerColor == YELLOW) {
            tft.setTextColor(YELLOW);
          } else if(computerColor == WHITE) {
            tft.setTextColor(WHITE);
          }    
          #endif
      }
      tft.setCursor(2*80+30, 2*80+25);
      tft.println(spaces[5]);
  
    //
  
      if(spaces[6] == player) {
        #ifdef playerColor
          if(playerColor == BLUE) {
            tft.setTextColor(BLUE);
          } else if(playerColor == RED) {
            tft.setTextColor(RED);
          } else if(playerColor == GREEN) {
            tft.setTextColor(GREEN);
          } else if(playerColor == CYAN) {
            tft.setTextColor(CYAN);
          } else if(playerColor == MAGENTA) {
            tft.setTextColor(MAGENTA);
          } else if(playerColor == YELLOW) {
            tft.setTextColor(YELLOW);
          } else if(playerColor == WHITE) {
            tft.setTextColor(WHITE);
          }    
        #endif
      } else {
          #ifdef computerColor 
            if(computerColor == BLUE) {
            tft.setTextColor(BLUE);
          } else if(computerColor == RED) {
            tft.setTextColor(RED);
          } else if(computerColor == GREEN) {
            tft.setTextColor(GREEN);
          } else if(computerColor == CYAN) {
            tft.setTextColor(CYAN);
          } else if(computerColor == MAGENTA) {
            tft.setTextColor(MAGENTA);
          } else if(computerColor == YELLOW) {
            tft.setTextColor(YELLOW);
          } else if(computerColor == WHITE) {
            tft.setTextColor(WHITE);
          }    
          #endif
      }
      tft.setCursor(0*80+30, 3*80+25);
      tft.println(spaces[6]);
      if(spaces[7] == player) {
        #ifdef playerColor
          if(playerColor == BLUE) {
            tft.setTextColor(BLUE);
          } else if(playerColor == RED) {
            tft.setTextColor(RED);
          } else if(playerColor == GREEN) {
            tft.setTextColor(GREEN);
          } else if(playerColor == CYAN) {
            tft.setTextColor(CYAN);
          } else if(playerColor == MAGENTA) {
            tft.setTextColor(MAGENTA);
          } else if(playerColor == YELLOW) {
            tft.setTextColor(YELLOW);
          } else if(playerColor == WHITE) {
            tft.setTextColor(WHITE);
          }    
        #endif
      } else {
          #ifdef computerColor 
            if(computerColor == BLUE) {
            tft.setTextColor(BLUE);
          } else if(computerColor == RED) {
            tft.setTextColor(RED);
          } else if(computerColor == GREEN) {
            tft.setTextColor(GREEN);
          } else if(computerColor == CYAN) {
            tft.setTextColor(CYAN);
          } else if(computerColor == MAGENTA) {
            tft.setTextColor(MAGENTA);
          } else if(computerColor == YELLOW) {
            tft.setTextColor(YELLOW);
          } else if(computerColor == WHITE) {
            tft.setTextColor(WHITE);
          }    
          #endif
      }
      tft.setCursor(1*80+30, 3*80+25);
      tft.println(spaces[7]);
      if(spaces[8] == player) {
        #ifdef playerColor
          if(playerColor == BLUE) {
            tft.setTextColor(BLUE);
          } else if(playerColor == RED) {
            tft.setTextColor(RED);
          } else if(playerColor == GREEN) {
            tft.setTextColor(GREEN);
          } else if(playerColor == CYAN) {
            tft.setTextColor(CYAN);
          } else if(playerColor == MAGENTA) {
            tft.setTextColor(MAGENTA);
          } else if(playerColor == YELLOW) {
            tft.setTextColor(YELLOW);
          } else if(playerColor == WHITE) {
            tft.setTextColor(WHITE);
          }    
        #endif
      } else {
          #ifdef computerColor 
            if(computerColor == BLUE) {
            tft.setTextColor(BLUE);
          } else if(computerColor == RED) {
            tft.setTextColor(RED);
          } else if(computerColor == GREEN) {
            tft.setTextColor(GREEN);
          } else if(computerColor == CYAN) {
            tft.setTextColor(CYAN);
          } else if(computerColor == MAGENTA) {
            tft.setTextColor(MAGENTA);
          } else if(computerColor == YELLOW) {
            tft.setTextColor(YELLOW);
          } else if(computerColor == WHITE) {
            tft.setTextColor(WHITE);
          }    
          #endif
      }
      tft.setCursor(2*80+30, 3*80+25);
      tft.println(spaces[8]);
      }
  #endif
}


void drawBoard(char *spaces) {
  
  tft.fillScreen(BLACK);
  tft.setTextColor(CYAN);
  tft.setTextSize(4);
  tft.setCursor(10, 25);
  
   #ifdef TITLEcolor
      if(TITLEcolor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(TITLEcolor == RED) {
        tft.setTextColor(RED);
      } else if(TITLEcolor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(TITLEcolor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(TITLEcolor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(TITLEcolor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(TITLEcolor == WHITE) {
        tft.setTextColor(WHITE);
      }  
      
      if(TITLEcolor == RAINBOW) {
        rainbow();
      }
    #endif
  
  tft.println("TikTakToe");
  tft.drawLine(0, 80, 240, 80, WHITE);
  tft.drawLine(1, 80 , 1, 320, WHITE);
  tft.drawLine(1*80, 80 , 1*80, 320, WHITE);
  tft.drawLine(2*80, 80 , 2*80, 320, WHITE);
  tft.drawLine(239, 80 , 239, 320, WHITE);
  tft.drawLine(0, 2*80, 320, 2*80, WHITE);
  tft.drawLine(0, 3*80, 320, 3*80, WHITE);
  tft.drawLine(0, 4*80-1, 320, 4*80-1, WHITE);
  
  tft.setTextSize(4);
  if(spaces[0] == player) {
    #ifdef playerColor
      if(playerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(playerColor == RED) {
        tft.setTextColor(RED);
      } else if(playerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(playerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(playerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(playerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(playerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
    #endif
  } else {
      #ifdef computerColor 
        if(computerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(computerColor == RED) {
        tft.setTextColor(RED);
      } else if(computerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(computerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(computerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(computerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(computerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
      #endif
  }
    tft.setCursor(0*80+30, 1*80+25);
    tft.println(spaces[0]);
    if(spaces[1] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
  }
    tft.setCursor(1*80+30, 1*80+25);
    tft.println(spaces[1]);
  if(spaces[2] == player) {
    #ifdef playerColor
      if(playerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(playerColor == RED) {
        tft.setTextColor(RED);
      } else if(playerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(playerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(playerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(playerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(playerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
    #endif
  } else {
      #ifdef computerColor 
        if(computerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(computerColor == RED) {
        tft.setTextColor(RED);
      } else if(computerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(computerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(computerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(computerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(computerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
      #endif
  }
    tft.setCursor(2*80+30, 1*80+25);
    tft.println(spaces[2]);

  //

    if(spaces[3] == player) {
    #ifdef playerColor
      if(playerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(playerColor == RED) {
        tft.setTextColor(RED);
      } else if(playerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(playerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(playerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(playerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(playerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
    #endif
  } else {
      #ifdef computerColor 
        if(computerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(computerColor == RED) {
        tft.setTextColor(RED);
      } else if(computerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(computerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(computerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(computerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(computerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
      #endif
  }
    tft.setCursor(0*80+30, 2*80+25);
    tft.println(spaces[3]);
    if(spaces[4] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(1*80+30, 2*80+25);
    tft.println(spaces[4]);
    if(spaces[5] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(2*80+30, 2*80+25);
    tft.println(spaces[5]);

  //

    if(spaces[6] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(0*80+30, 3*80+25);
    tft.println(spaces[6]);
    if(spaces[7] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(1*80+30, 3*80+25);
    tft.println(spaces[7]);
    if(spaces[8] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(2*80+30, 3*80+25);
    tft.println(spaces[8]);
}

void playerMove(char *spaces, char player) {
  TSPoint p = ts.getPoint();
  while(true) {
    rainbow();
    TSPoint p = ts.getPoint();
    //Serial.print(p.x); Serial.print("        "); Serial.println(p.y);
    //1x1
    if(Touch_getXY() && p.x < 390 && p.x > 160 && p.y < 760 && p.y > 570 && p.z > MINPRESSURE && p.z < MAXPRESSURE && spaces[0] == ' ') {
      if(spaces[0] == ' ') {
      spaces[0] = player;
      break;
      }
    }
    //1x2
    if(Touch_getXY() && p.x < 670 && p.x > 400 && p.y < 760 && p.y > 570 && p.z > MINPRESSURE && p.z < MAXPRESSURE && spaces[1] == ' ') {
      if(spaces[1] == ' ') {
      spaces[1] = player;
      break;
      }
    }
    //1x3
    if(Touch_getXY() && p.x < 920 && p.x > 770 && p.y < 760 && p.y > 570 && p.z > MINPRESSURE && p.z < MAXPRESSURE  && spaces[2] == ' ') {
      if(spaces[2] == ' ') {
      spaces[2] = player;
      break;
      }
    }
    //2x1
    if(Touch_getXY() && p.x < 390 && p.x > 160 && p.y < 570 && p.y > 370 && p.z > MINPRESSURE && p.z < MAXPRESSURE && spaces[3] == ' ') {
      if(spaces[3] == ' ') {
      spaces[3] = player;
      break;
      }
    }
    //2x2
    if(Touch_getXY() && p.x < 670 && p.x > 400 && p.y < 570 && p.y > 370 && p.z > MINPRESSURE && p.z < MAXPRESSURE && spaces[4] == ' ') {
      if(spaces[4] == ' ') {
      spaces[4] = player;
      break;
      }
    }
    //2x3
    if(Touch_getXY() && p.x < 920 && p.x > 770 && p.y < 570 && p.y > 370 && p.z > MINPRESSURE && p.z < MAXPRESSURE && spaces[5] == ' ') {
      if(spaces[5] == ' ') {
      spaces[5] = player;
      break;
      }
    }
    //3x1
    if(Touch_getXY() && p.x < 390 && p.x > 160 && p.y < 370 && p.y > 170 && p.z > MINPRESSURE && p.z < MAXPRESSURE && spaces[6] == ' ') {
      if(spaces[6] == ' ') {
      spaces[6] = player;
      break;
      }
    }
    //2x2
    if(Touch_getXY() && p.x < 670 && p.x > 400 && p.y < 370 && p.y > 170 && p.z > MINPRESSURE && p.z < MAXPRESSURE && spaces[7] == ' ') {
      if(spaces[7] == ' ') {
      spaces[7] = player;
      break;
      }
    }
    //2x3
    if(Touch_getXY() && p.x < 920 && p.x > 770 && p.y < 370 && p.y > 170 && p.z > MINPRESSURE && p.z < MAXPRESSURE && spaces[8] == ' ') {
      if(spaces[8] == ' ') {
      spaces[8] = player;
      break;
      }
    }
  }
  while(Touch_getXY()) {TSPoint p = ts.getPoint();}

}

void computerMove(char *spaces, char computer) {
    int number;
    while(true)
    {   
        rainbow()
        number = random(0, 8);
        if((spaces[number] == ' ') && (number > 0) && (number < 8))
        {
            spaces[number] = computer;
            break;
        }
    }
}

bool checkTie(char *spaces) {
    for(int i = 0; i < 9; i++) {
        rainbow()
        if(spaces[i] == ' ') {
            return false;
        }
    }
    return true;
}

bool checkWinner(char *spaces, char player, char computer) {
    if(spaces[0] != ' ' && spaces[0] == spaces[1] && spaces[1] == spaces[2])
    {   
        winncase = 1;
        spaces[0] == player ? printWinner(spaces) : printLooser(spaces);
        
        return true;
    } else if(spaces[3] != ' ' && spaces[3] == spaces[4] && spaces[4] == spaces[5])
        {
             winncase = 2;
            spaces[3] == player ? printWinner(spaces) : printLooser(spaces);
       
            return true;
        }
      else if(spaces[6] != ' ' && spaces[6] == spaces[7] && spaces[7] == spaces[8])
        {   
            winncase = 3;
            spaces[6] == player ? printWinner(spaces) : printLooser(spaces);
        
            return true;
        }
      else if(spaces[0] != ' ' && spaces[0] == spaces[3] && spaces[3] == spaces[6])
        {   
            winncase = 4;
            spaces[0] == player ? printWinner(spaces) : printLooser(spaces);
        
            return true;
        }
      else if(spaces[1] != ' ' && spaces[1] == spaces[4] && spaces[4] == spaces[7])
        {
            winncase = 5;
            spaces[1] == player ? printWinner(spaces) : printLooser(spaces);
        
            return true;
        }
      else if(spaces[2] != ' ' && spaces[2] == spaces[5] && spaces[5] == spaces[8])
        {
            winncase = 6;
            spaces[2] == player ? printWinner(spaces) : printLooser(spaces);
        
            return true;
        }
      else if(spaces[0] != ' ' && spaces[0] == spaces[4] && spaces[4] == spaces[8])
        {
            winncase = 7;
            spaces[0] == player ? printWinner(spaces) : printLooser(spaces);
        
            return true;
        }
      else if(spaces[6] != ' ' && spaces[6] == spaces[4] && spaces[4] == spaces[2])
        {
            winncase = 8;
            spaces[6] == player ? printWinner(spaces) : printLooser(spaces);
        
            return true;
        }
    
   return false; 
}
void printWinner(char *spaces) {
  tft.fillScreen(BLACK);
  tft.setTextColor(GREEN);
  tft.setTextSize(4);
  tft.setCursor(90, 25);
  tft.println("WON");
  tft.drawLine(0, 80, 240, 80, WHITE);
  tft.drawLine(1, 80 , 1, 320, WHITE);
  tft.drawLine(1*80, 80 , 1*80, 320, WHITE);
  tft.drawLine(2*80, 80 , 2*80, 320, WHITE);
  tft.drawLine(239, 80 , 239, 320, WHITE);
  tft.drawLine(0, 2*80, 320, 2*80, WHITE);
  tft.drawLine(0, 3*80, 320, 3*80, WHITE);
  tft.drawLine(0, 4*80-1, 320, 4*80-1, WHITE);
  
  switch(winncase) {
      case 1:
        tft.drawLine(0, 1*80+40 , 320, 1*80+40, YELLOW);
      break;
      case 2:
        tft.drawLine(0, 2*80+40 , 320, 2*80+40, YELLOW);
      break;
      case 3:
        tft.drawLine(0, 3*80+40 , 320, 3*80+40, YELLOW);
      break;
      case 4:
        tft.drawLine(0*80+40, 1*80, 0*80+40, 320, YELLOW);
      break;
      case 5:
        tft.drawLine(1*80+40, 1*80, 1*80+40, 320, YELLOW);
      break;
      case 6:
        tft.drawLine(2*80+40, 1*80, 2*80+40, 320, YELLOW);
      break;
      case 7:
        tft.drawLine(0, 80, 240, 320, YELLOW);
      break;
      case 8:
        tft.drawLine(0, 320, 240, 80, YELLOW);
      break;
    }

  tft.setTextSize(4);
  if(spaces[0] == player) {
    #ifdef playerColor
      if(playerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(playerColor == RED) {
        tft.setTextColor(RED);
      } else if(playerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(playerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(playerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(playerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(playerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
    #endif
  } else {
      #ifdef computerColor 
        if(computerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(computerColor == RED) {
        tft.setTextColor(RED);
      } else if(computerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(computerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(computerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(computerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(computerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
      #endif
  }
    tft.setCursor(0*80+30, 1*80+25);
    tft.println(spaces[0]);
    if(spaces[1] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
  }
    tft.setCursor(1*80+30, 1*80+25);
    tft.println(spaces[1]);
  if(spaces[2] == player) {
    #ifdef playerColor
      if(playerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(playerColor == RED) {
        tft.setTextColor(RED);
      } else if(playerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(playerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(playerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(playerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(playerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
    #endif
  } else {
      #ifdef computerColor 
        if(computerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(computerColor == RED) {
        tft.setTextColor(RED);
      } else if(computerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(computerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(computerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(computerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(computerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
      #endif
  }
    tft.setCursor(2*80+30, 1*80+25);
    tft.println(spaces[2]);

  //

    if(spaces[3] == player) {
    #ifdef playerColor
      if(playerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(playerColor == RED) {
        tft.setTextColor(RED);
      } else if(playerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(playerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(playerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(playerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(playerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
    #endif
  } else {
      #ifdef computerColor 
        if(computerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(computerColor == RED) {
        tft.setTextColor(RED);
      } else if(computerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(computerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(computerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(computerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(computerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
      #endif
  }
    tft.setCursor(0*80+30, 2*80+25);
    tft.println(spaces[3]);
    if(spaces[4] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(1*80+30, 2*80+25);
    tft.println(spaces[4]);
    if(spaces[5] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(2*80+30, 2*80+25);
    tft.println(spaces[5]);

  //

    if(spaces[6] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(0*80+30, 3*80+25);
    tft.println(spaces[6]);
    if(spaces[7] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(1*80+30, 3*80+25);
    tft.println(spaces[7]);
    if(spaces[8] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(2*80+30, 3*80+25);
    tft.println(spaces[8]);
    
}

void printLooser(char *spaces) {
  tft.fillScreen(BLACK);
  tft.setTextColor(RED);
  tft.setTextSize(4);
  tft.setCursor(90, 25);
  tft.println("LOSE");
  tft.drawLine(0, 80, 240, 80, WHITE);
  tft.drawLine(1, 80 , 1, 320, WHITE);
  tft.drawLine(1*80, 80 , 1*80, 320, WHITE);
  tft.drawLine(2*80, 80 , 2*80, 320, WHITE);
  tft.drawLine(239, 80 , 239, 320, WHITE);
  tft.drawLine(0, 2*80, 320, 2*80, WHITE);
  tft.drawLine(0, 3*80, 320, 3*80, WHITE);
  tft.drawLine(0, 4*80-1, 320, 4*80-1, WHITE);
  
  tft.setTextSize(4);
  if(spaces[0] == player) {
    #ifdef playerColor
      if(playerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(playerColor == RED) {
        tft.setTextColor(RED);
      } else if(playerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(playerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(playerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(playerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(playerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
    #endif
  } else {
      #ifdef computerColor 
        if(computerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(computerColor == RED) {
        tft.setTextColor(RED);
      } else if(computerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(computerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(computerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(computerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(computerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
      #endif
  }
    tft.setCursor(0*80+30, 1*80+25);
    tft.println(spaces[0]);
    if(spaces[1] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
  }
    tft.setCursor(1*80+30, 1*80+25);
    tft.println(spaces[1]);
  if(spaces[2] == player) {
    #ifdef playerColor
      if(playerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(playerColor == RED) {
        tft.setTextColor(RED);
      } else if(playerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(playerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(playerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(playerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(playerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
    #endif
  } else {
      #ifdef computerColor 
        if(computerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(computerColor == RED) {
        tft.setTextColor(RED);
      } else if(computerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(computerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(computerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(computerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(computerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
      #endif
  }
    tft.setCursor(2*80+30, 1*80+25);
    tft.println(spaces[2]);

  //

    if(spaces[3] == player) {
    #ifdef playerColor
      if(playerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(playerColor == RED) {
        tft.setTextColor(RED);
      } else if(playerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(playerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(playerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(playerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(playerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
    #endif
  } else {
      #ifdef computerColor 
        if(computerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(computerColor == RED) {
        tft.setTextColor(RED);
      } else if(computerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(computerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(computerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(computerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(computerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
      #endif
  }
    tft.setCursor(0*80+30, 2*80+25);
    tft.println(spaces[3]);
    if(spaces[4] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(1*80+30, 2*80+25);
    tft.println(spaces[4]);
    if(spaces[5] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(2*80+30, 2*80+25);
    tft.println(spaces[5]);

  //

    if(spaces[6] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(0*80+30, 3*80+25);
    tft.println(spaces[6]);
    if(spaces[7] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(1*80+30, 3*80+25);
    tft.println(spaces[7]);
    if(spaces[8] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(2*80+30, 3*80+25);
    tft.println(spaces[8]);
    switch(winncase) {
      case 1:
        tft.drawLine(0, 1*80+40 , 320, 1*80+40, YELLOW);
      break;
      case 2:
        tft.drawLine(0, 2*80+40 , 320, 2*80+40, YELLOW);
      break;
      case 3:
        tft.drawLine(0, 3*80+40 , 320, 3*80+40, YELLOW);
      break;
      case 4:
        tft.drawLine(0*80+40, 1*80, 0*80+40, 320, YELLOW);
      break;
      case 5:
        tft.drawLine(1*80+40, 1*80, 1*80+40, 320, YELLOW);
      break;
      case 6:
        tft.drawLine(2*80+40, 1*80, 2*80+40, 320, YELLOW);
      break;
      case 7:
        tft.drawLine(0, 80, 240, 320, YELLOW);
      break;
      case 8:
        tft.drawLine(0, 320, 240, 80, YELLOW);
      break;
    }
}

void printTie(char *spaces) {
  tft.fillScreen(BLACK);
  tft.setTextColor(YELLOW);
  tft.setTextSize(4);
  tft.setCursor(90, 25);
  tft.println("TIE");
  tft.drawLine(0, 80, 240, 80, WHITE);
  tft.drawLine(1, 80 , 1, 320, WHITE);
  tft.drawLine(1*80, 80 , 1*80, 320, WHITE);
  tft.drawLine(2*80, 80 , 2*80, 320, WHITE);
  tft.drawLine(239, 80 , 239, 320, WHITE);
  tft.drawLine(0, 2*80, 320, 2*80, WHITE);
  tft.drawLine(0, 3*80, 320, 3*80, WHITE);
  tft.drawLine(0, 4*80-1, 320, 4*80-1, WHITE);
  
  tft.setTextSize(4);
  if(spaces[0] == player) {
    #ifdef playerColor
      if(playerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(playerColor == RED) {
        tft.setTextColor(RED);
      } else if(playerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(playerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(playerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(playerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(playerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
    #endif
  } else {
      #ifdef computerColor 
        if(computerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(computerColor == RED) {
        tft.setTextColor(RED);
      } else if(computerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(computerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(computerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(computerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(computerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
      #endif
  }
    tft.setCursor(0*80+30, 1*80+25);
    tft.println(spaces[0]);
    if(spaces[1] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
  }
    tft.setCursor(1*80+30, 1*80+25);
    tft.println(spaces[1]);
  if(spaces[2] == player) {
    #ifdef playerColor
      if(playerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(playerColor == RED) {
        tft.setTextColor(RED);
      } else if(playerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(playerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(playerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(playerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(playerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
    #endif
  } else {
      #ifdef computerColor 
        if(computerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(computerColor == RED) {
        tft.setTextColor(RED);
      } else if(computerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(computerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(computerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(computerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(computerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
      #endif
  }
    tft.setCursor(2*80+30, 1*80+25);
    tft.println(spaces[2]);

  //

    if(spaces[3] == player) {
    #ifdef playerColor
      if(playerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(playerColor == RED) {
        tft.setTextColor(RED);
      } else if(playerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(playerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(playerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(playerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(playerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
    #endif
  } else {
      #ifdef computerColor 
        if(computerColor == BLUE) {
        tft.setTextColor(BLUE);
      } else if(computerColor == RED) {
        tft.setTextColor(RED);
      } else if(computerColor == GREEN) {
        tft.setTextColor(GREEN);
      } else if(computerColor == CYAN) {
        tft.setTextColor(CYAN);
      } else if(computerColor == MAGENTA) {
        tft.setTextColor(MAGENTA);
      } else if(computerColor == YELLOW) {
        tft.setTextColor(YELLOW);
      } else if(computerColor == WHITE) {
        tft.setTextColor(WHITE);
      }    
      #endif
  }
    tft.setCursor(0*80+30, 2*80+25);
    tft.println(spaces[3]);
    if(spaces[4] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(1*80+30, 2*80+25);
    tft.println(spaces[4]);
    if(spaces[5] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(2*80+30, 2*80+25);
    tft.println(spaces[5]);

  //

    if(spaces[6] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(0*80+30, 3*80+25);
    tft.println(spaces[6]);
    if(spaces[7] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(1*80+30, 3*80+25);
    tft.println(spaces[7]);
    if(spaces[8] == player) {
      #ifdef playerColor
        if(playerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(playerColor == RED) {
          tft.setTextColor(RED);
        } else if(playerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(playerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(playerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(playerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(playerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
      #endif
    } else {
        #ifdef computerColor 
          if(computerColor == BLUE) {
          tft.setTextColor(BLUE);
        } else if(computerColor == RED) {
          tft.setTextColor(RED);
        } else if(computerColor == GREEN) {
          tft.setTextColor(GREEN);
        } else if(computerColor == CYAN) {
          tft.setTextColor(CYAN);
        } else if(computerColor == MAGENTA) {
          tft.setTextColor(MAGENTA);
        } else if(computerColor == YELLOW) {
          tft.setTextColor(YELLOW);
        } else if(computerColor == WHITE) {
          tft.setTextColor(WHITE);
        }    
        #endif
    }
    tft.setCursor(2*80+30, 3*80+25);
    tft.println(spaces[8]);
}