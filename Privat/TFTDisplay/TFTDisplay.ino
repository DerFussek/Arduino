#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define TFT_SCLK 45
#define TFT_MOSI 43
#define TFT_CS  37
#define TFT_RST 41
#define TFT_DC 39

#define SW1 53
#define SW2 51
#define SW3 49
#define SW4 47

#define LED 35        
#define Transistor 33 

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

#define DEV
void T() {
  Serial.print(F("Testing LED..."));
  digitalWrite(LED, HIGH);
  Serial.print(F("."));
  delay(1000);
  digitalWrite(LED, LOW);
  Serial.print(F("."));
  delay(1000);
  digitalWrite(LED, HIGH);
  Serial.print(F("."));
  delay(500);
  digitalWrite(LED, LOW);
  Serial.print(F("."));
  Serial.println(F("Done."));
  delay(1000);
  Serial.print(F("Testing Display..."));
  tft.fillScreen(ST77XX_BLACK);
  delay(50);
  tft.fillScreen(ST77XX_RED);
  delay(50);
  tft.fillScreen(ST77XX_GREEN);
  delay(50);
  tft.fillScreen(ST77XX_BLUE);
  delay(50);
  tft.fillScreen(ST77XX_MAGENTA);
  delay(50);
  tft.fillScreen(ST77XX_YELLOW);
  delay(50);
  tft.fillScreen(ST77XX_CYAN);
  delay(50);
  tft.fillScreen(ST77XX_ORANGE);
  delay(50);
  tft.fillScreen(ST77XX_WHITE);
  delay(50);
  tft.fillScreen(ST77XX_BLACK);
}

void setup() {
  
  Serial.begin(9600);
  
  Serial.print(F("Hello! ST7735 TFT Test"));
  tft.initR(INITR_GREENTAB);
  Serial.println(F("Initialized"));
  tft.fillScreen(ST77XX_BLACK);

  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP);

  pinMode(LED, OUTPUT);
  pinMode(Transistor, OUTPUT);

  #ifdef DEV
    T();
  #endif
}

void loop() {
  // put your main code here, to run repeatedly:

}
