#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <Adafruit_ST7789.h> 
#include <SPI.h>
#include <Wire.h>
#include <DS3231.h>

// Display-Pins
#define TFT_CS    25
#define TFT_RST   23
#define TFT_DC    27
#define TFT_MOSI  29
#define TFT_SCLK  31

// Display- und RTC-Objekte
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
DS3231 myRTC;
bool h12, PM, century;
String wochentag[] {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};

// Kreis-Zeichenfunktion
void drawArc(int x, int y, int r, int startAngle, int endAngle, int thickness, uint16_t color) {
  for (int angle = startAngle - 90; angle <= endAngle - 90; angle++) {
    float rad = angle * 3.14159 / 180.0;
    for (int t = 0; t < thickness; t++) {
      int px = x + (r - t) * cos(rad);
      int py = y + (r - t) * sin(rad);
      tft.drawPixel(px, py, color);
    }
  }
}

void setup(void) {
  Wire.begin();
  Serial.begin(9600);

  tft.initR(INITR_GREENTAB);
  
  tft.fillScreen(ST77XX_BLACK);
  delay(500);
  // Statische Anzeige beim Start
  tft.setTextColor(tft.color565(0, 255, 165));
  tft.setTextSize(4);

  // Uhrzeit berechnen & anzeigen
  byte hour = myRTC.getHour(h12, PM);
  byte minute = myRTC.getMinute();
  String uhrTxt = (hour < 10 ? "0" : "") + String(hour) + ":" +
                  (minute < 10 ? "0" : "") + String(minute);
  const char* uhrTxtC = uhrTxt.c_str();

  int16_t _uhrX, _uhrY;
  uint16_t uhrW, uhrH;
  tft.getTextBounds(uhrTxtC, 0, 0, &_uhrX, &_uhrY, &uhrW, &uhrH);
  int16_t uhrX = (128 - uhrW) / 2 - _uhrX + 3;
  int16_t uhrY = (160 - uhrH) / 2 - _uhrY + 26;
  tft.setCursor(uhrX, uhrY);
  tft.print(uhrTxtC);

  // Datum anzeigen
  byte day = myRTC.getDate();
  byte month = myRTC.getMonth(century);
  byte year = myRTC.getYear();
  String dateTxt = (day < 10 ? "0" : "") + String(day) + "." +
                   (month < 10 ? "0" : "") + String(month) + "." +
                   "20" + String(year);
  const char* dateTxtC = dateTxt.c_str();

  tft.setTextColor(tft.color565(255, 255, 255));
  tft.setTextSize(1);

  int16_t _dateX, _dateY;
  uint16_t dateW, dateH;
  tft.getTextBounds(dateTxtC, 0, 0, &_dateX, &_dateY, &dateW, &dateH);
  int16_t dateX = (128 - dateW) / 2 - _dateX;
  int16_t dateY = uhrY + uhrH + 5;
  tft.setCursor(dateX, dateY);
  tft.print(dateTxtC);

  //Wochentag
  tft.setTextColor(tft.color565(255, 255, 255));
  tft.setTextSize(1);

  String DowTxt = wochentag[myRTC.getDoW()];
  int16_t _DowX, _DowY;
  uint16_t DowW, DowH;
  const char* DowTxtC = DowTxt.c_str();
  tft.getTextBounds(DowTxtC, 0, 0, &_DowX, &_DowY, &DowW, &DowH);
  int16_t DowX = (128 - DowW) / 2 - _DowX;
  int16_t DowY = uhrY - 15;
  tft.setCursor(DowX, DowY);
  tft.print(DowTxtC);

  tft.setTextSize(2);
  // Kreise
  int16_t circleR = 20;
  // Links
  tft.fillCircle(32, 38, circleR, tft.color565(3, 26, 12));
  tft.fillCircle(32, 38, circleR - 3, tft.color565(0, 0, 0));
  drawArc(32, 38, circleR, 0, 180, 3, tft.color565(0, 255, 165));
  tft.setCursor(27, 32);
  tft.println("3");
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setCursor(16, 62);
  tft.print("Stufe");

  // Rechts
  tft.fillCircle(96, 38, circleR, tft.color565(0, 25, 50));
  tft.fillCircle(96, 38, circleR - 3, tft.color565(0, 0, 0));
  drawArc(96, 38, circleR, 0, 250, 3, tft.color565(0, 128, 255));
  tft.setCursor(86, 32);
  tft.println("00");
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setCursor(84, 62);
  tft.print("Temp.");
}

void loop() {
  static String lastTime = "";
  static byte lastTemp = "";

  byte hour = myRTC.getHour(h12, PM);
  byte minute = myRTC.getMinute();
  String uhrTxt = (hour < 10 ? "0" : "") + String(hour) + ":" +
                  (minute < 10 ? "0" : "") + String(minute);
  byte temp = myRTC.getTemperature();

  if (uhrTxt != lastTime) {
    lastTime = uhrTxt;

    // Hintergrundfläche für Uhrzeit löschen
    tft.fillRect(0, 85, 128, 35, ST77XX_BLACK);

    tft.setTextColor(tft.color565(0, 255, 165));
    tft.setTextSize(4);

    const char* uhrTxtC = uhrTxt.c_str();
    int16_t _uhrX, _uhrY;
    uint16_t uhrW, uhrH;
    tft.getTextBounds(uhrTxtC, 0, 0, &_uhrX, &_uhrY, &uhrW, &uhrH);
    int16_t uhrX = (128 - uhrW) / 2 - _uhrX;
    int16_t uhrY = (160 - uhrH) / 2 - _uhrY + 26;
    tft.setCursor(uhrX, uhrY);
    tft.print(uhrTxtC);

  } else if(abs(temp - lastTemp) > 1) {
    lastTemp = temp;
    float minTemp = 0;
    float maxTemp = 40;
    float winkelF = map(temp, minTemp, maxTemp, 0, 360);
    int winkel = constrain(round(winkelF), 0, 360);

    tft.fillRect(76, 18, 2*20, 2*20, ST7735_BLACK);
    tft.fillCircle(96, 38, 20, tft.color565(0, 25, 50));
    tft.fillCircle(96, 38, 20 - 3, tft.color565(0, 0, 0));
    drawArc(96, 38, 20, 0, winkel, 3, tft.color565(0, 128, 255));
    tft.setTextSize(2);
    tft.setTextColor(tft.color565(255, 255, 255));
    tft.setCursor(86, 32);
    tft.println(temp);
  }

  delay(200);
}
