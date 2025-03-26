//===============================================================================================//
                                    //Projekt Sonnenverfolger
//===============================================================================================//
/*
  Anforderungen:
      Display:
        - Spannung Photovoltaik Modul
        - Drehwinkel
        - Azimut
        - Ansprechendes Design
      Motor:
        - Soll mit hilfe zwei Lichtsensoren die Sonne verfolgen
*/
//===========================================//
                //Display
//===========================================//
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h> 
#define SCREEN_WIDTH 128 //Display breite
#define SCREEN_HEIGHT 64  //Display höhe
#define OLED_RESET -1 //Display benutzt zum Zurücksetzen keinen Extra pin, deshalb bekommt diese Definition den Wert -1 zugewiesen.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Hier wird die Klasse, die für das Display zuständig ist, initialisiert.
/*
  Symbol für die Spannung, hier ein Blitz, in der Form einer Bitmap. 
  Eine Bitmap ist eine Anordung von Bits, die eine Grafik repräsentieren sollen. Hier sind die Bits
  in einem Array gespeichert.
  Das Array wird hier als const unsigned deklariert, das sich die Werte im Array nicht ändern sollen und diese nicht negativ werden sollen.
  Das PROGREM Schlüsselwort sorgt dafür, dass das Array im Programmspeicher (Flash Memory) und nicht im sogenannten SRam (Flüchtigem Speicher)
  gespeichtert wird.
  Das ist wichtig, da der flüchtige Speicher begrenzt ist und im vergleich zum Programmspeicher sehr viel kleiner ist.
  Der flüchtige Speicher wird für dynamische Variablen benutzt, wie zum Beispiel bei einer Variable, die die Spannung des
  Solarmoduls speichert oder die Position eines Schrittmotors. Im Programmspeicher wird, wie der Name schon sagt,
  dass Programm gespeichert oder statische Variablen.
*/
// 'volt symbol', 32x32px
const unsigned char menu_icon_blitz [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0xc0, 0x04, 0x00, 
  0x00, 0xc0, 0x18, 0x00, 0x00, 0xc0, 0x18, 0x00, 0x01, 0x00, 0x60, 0x00, 0x01, 0x00, 0xff, 0x80, 
  0x01, 0x00, 0xff, 0x80, 0x06, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0xc0, 0x06, 0x00, 0x00, 0x80, 
  0x01, 0xf8, 0x03, 0x00, 0x01, 0xfc, 0x06, 0x00, 0x00, 0x06, 0x0c, 0x00, 0x00, 0x06, 0x18, 0x00, 
  0x00, 0x0c, 0x10, 0x00, 0x00, 0x18, 0x60, 0x00, 0x00, 0x18, 0xc0, 0x00, 0x00, 0x18, 0x80, 0x00, 
  0x00, 0x23, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*
  Symbol für den Drehwinkel des Sonnenverfolgers
*/
// 'grad symbol', 32x32px
const unsigned char menu_icon_grad [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x78, 0x3c, 0x00, 
  0x00, 0xc0, 0x07, 0x00, 0x03, 0x80, 0x01, 0x90, 0x06, 0x00, 0x00, 0xf0, 0x0c, 0x00, 0x00, 0xf0, 
  0x08, 0x00, 0x01, 0xf0, 0x18, 0x00, 0x00, 0x70, 0x10, 0x00, 0x00, 0x10, 0x30, 0x00, 0x00, 0x08, 
  0x23, 0xc1, 0x8f, 0x0c, 0x20, 0x66, 0x09, 0x0c, 0x20, 0x64, 0x19, 0x84, 0x60, 0x47, 0x90, 0x84, 
  0x61, 0xcc, 0xd0, 0x84, 0x20, 0x6c, 0x50, 0x84, 0x20, 0x2c, 0x51, 0x8c, 0x20, 0x24, 0x59, 0x0c, 
  0x33, 0xc7, 0x8f, 0x08, 0x11, 0x81, 0x04, 0x08, 0x1c, 0x00, 0x00, 0x18, 0x1f, 0x00, 0x00, 0x30, 
  0x1f, 0x00, 0x00, 0x20, 0x1e, 0x00, 0x00, 0x60, 0x1f, 0x00, 0x00, 0xc0, 0x01, 0x80, 0x03, 0x80, 
  0x00, 0xf0, 0x0e, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*
  Symbol für den Azimut des Sonnenverfolgers
*/
// 'azimut symbol', 32x32px
const unsigned char menu_icon_kompass [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 
  0x00, 0x7f, 0xfe, 0x00, 0x07, 0xc0, 0x03, 0xe0, 0x07, 0xc0, 0x03, 0xe0, 0x18, 0x00, 0x00, 0x18, 
  0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x1e, 0x78, 0x00, 0x3c, 0x06, 0x60, 0x00, 0x3c, 0x06, 
  0x60, 0x00, 0xf0, 0x06, 0x60, 0x00, 0xf0, 0x06, 0x60, 0x03, 0xc0, 0x06, 0x60, 0x03, 0xc0, 0x06, 
  0x7c, 0x03, 0xc0, 0x3e, 0x7c, 0x03, 0xc0, 0x3e, 0x67, 0xc0, 0x03, 0xe6, 0x67, 0xc0, 0x03, 0xe6, 
  0x60, 0x3f, 0xfc, 0x06, 0x78, 0x3f, 0xfc, 0x06, 0x18, 0x00, 0x00, 0x1e, 0x18, 0x00, 0x00, 0x18, 
  0x18, 0x00, 0x00, 0x18, 0x07, 0xc0, 0x03, 0xe0, 0x07, 0xc0, 0x03, 0xe0, 0x00, 0x3f, 0xfe, 0x00, 
  0x00, 0x3f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//===========================================//
              //Schrittmotor
//===========================================//
#include <Stepper.h> 
#define SPU 2048 //Definiert die Anzahl der Schritte, die Der Schrittmotor für eine volle Umdrehung benötigt (Bei jedem Motor anders).
#define in1 6 // Hier sind die vier Pins, mit denen der Arduino dem Schirttmotor befehle gibt, angegeben.
#define in2 5
#define in3 4
#define in4 3

Stepper stepper(SPU, in4, in2, in3, in1); //Hier wird die Klasse, die für die Steuerung des Schrittmotors zuständig ist, initialisiert.

bool stillStand = false;  //Diese Variable gibt an, ob der Sonnenverfolger sich bewegt oder nicht. Deshalb ist es auch eine boolische Variable.
int motorPosition = 0;  //Gibt die Position des Motors an. Die Nullposition ändert sich nach jedem Neustart des Codes. 

//===========================================//
        //Allg. Variablen & Definitionen
//===========================================//
#define LS1 A2  //Hier wird der Pin für den ersten Lcihtsensor definiert (hier A2). 
#define LS2 A1  //
#define PVMod A0  //Hier wird der Pin für das Photovoltaikmodul definiert (hier A0).

int slide = 1;  //Diese Variable gibt an, welchen Menüpunkt das Display grade anzeigt
unsigned long sldAnzL = millis(); //Diese variable ist mit dafür Verantwortlich, wie lang eine Menüpunkt angezeigt wird.

//===========================================//
            //Ein-/Ausschalter
//===========================================//
#define SW 2  //Definiert den Pin, an dem der Taster am Arduino angeschlossen ist (hier Pin 2).
bool Stopp = false; //Wenn diese boolische Variable den Wert 1/True hat, geht der Sonnenverfolgen in den Aus Zustand.

/*
  Diese Funktion wird nur dann ausgeführt, wenn jemand den Taster drückt. Wenn der Taster gedrückt wird, wrid die boolische Variable Stopp invertiert. Danach kann man den Taster erst wieder nach einer
  Sekunde drücken.
*/
void onPress() {
  static unsigned long leUnZeit = 0;  //Wird als static unsigned deklariert, da diese Variable sich nicht mit jedem Tasterdruck wieder zurücksetzen soll.
  unsigned long UnZeit = millis();  //wird als unsigned long deklariert, um eine längere Zeit in der Variable speichern zu können.
  if(UnZeit - leUnZeit > 1000) {
    Stopp = !Stopp; //Stopp Variable wird invertiert.
  }
  
  leUnZeit = UnZeit;    //Delay wird Zurückgesetzt
}

//===========================================//
                //Setup
//===========================================//
void setup() {
  Serial.begin(9600); //Serieller Monitor wird mit einer Baudrate von 9600 gestartet
  
  pinMode(SW, INPUT_PULLUP);  //Der Pin, an dem der Taster angeschlossen ist, wird auf den INPUT_PULLUP modus gesetzt.
  attachInterrupt(0, onPress, FALLING); //Diese Zeile konfiguriert einen Interrupt am Pin zwei (erstes Argument) 
                                        //und führt die onPress Funktion aus (zweites Argument),
                                        //wenn sich der Wert von HIGH/1 auf LOW/0 ändert (drittes Argument). 
  Serial.print("Display wird vorbereitet...");  //Lässt den Text "Display wird vorbereitet..." auf dem Seriellen Monitor anzeigen-
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {    //Diese If-Schleife wird ausgeführt, wenn keine Verbindung mit dem Display aufgenommen werden konnte.
      Serial.println(F("SSD1306 allocation failed")); //Ist dies der Fall, wird "SSD1306 allocation failed" auf dem Seriellen Monitor angezeigt. 
                                                      //Dies wird mit der F() Funktion gemacht, um Programmspeicher zu sparen.
      for(;;);                                        //Danach wird eine endlos-Schleife ausgeführt.
  } 
  delay(500); //es wird eine halbe Sekunde gewartet, bis das Display bereit ist befehle zu empfangen.
  display.clearDisplay(); //Inhalt auf dem Display wird gelöscht
  display.setTextSize(1); //Textgröße wird festgelegt
  display.setTextColor(WHITE); //Textfarbe wird festgelegt
  display.setCursor(0, 10);           //Der Text "Sonnenverfolger" wird auf der Position 0:10, des Displays, angezeigt. 
  display.println("Sonnenverfolger"); //In der nächsten Zeile wird dann der Text "Code, by Til" angezeigt.
  display.println("Lasse, Luca, Piet, Til "); 
  display.display();  //Display wird aktuallisiert

  Serial.println("Fertig"); //der Text "Fertig", wird auf dem Seriellen monitor angezeigt.
  stepper.setSpeed(10); //Die Geschwindigkeit des Schrittmotors wird auf 10 festgelegt.
}

//===========================================//
                  //Loop
//===========================================//
/*
  Wenn die boolische Variable Stopp den wert null hat, werden die Funktionen stepperUpdate() und updateDisplay() ausgeführt.
  Hat die Variable den wert eins, wird der Inhalt auf dem Display gelöscht und die genannten Funktionen werden nicht mehr ausgeführt.
*/
void loop() {
  if(!Stopp) {    //wird ausgefürht wenn der Wert der boolischen Variable 0/false beträgt
    stepperUpdate();  //Unterprogramm stepperUpdate wird ausgeführt
    updateDisplay();  //Unterprogramm updateDisplay wird ausgeführt
  } else {                  //wenn die Variable einen Wert von 1/true hat, 
    display.clearDisplay(); //Inhalt des Display wird gelöscht
    display.display();  //Display wird geupdatet
  }
}

//===========================================//
              //Schrittmotor
//===========================================//
/*
  In dieser Funktion werden die Werte der zwei Lichtsensoren in Variablen (inS1 & inS2) gespeichert.
  Von diesen zwei Werten wird die Differenz berechnet und in der lokalen Variable res gespeichert. 
  Liegt diese Differenz nicht zwischen einem gewissen Bereich, hier von -10 bis 10, wird der Schrittmotor so lange
  gedreht, bis diese in dem gennanten Bereich liegt. Die Drehrichtung hängt hierbei davon ab, ob die Differenz positiv oder 
  negativ ist.
  Liegt die Variable res in dem definierten Bereich, wird zusätzlich die bolische Variable stillStand auf 1/true gesetzt.
  Ist dies nicht der Fall, wird die Variable auf 0/false gesetzt. Warum Ich diese Varialbe benötige, erkläre ich bei der updateDisplay Funktion.
  Außerdem wird die Variable motorPosition nach jeder Bewegung des Schrittmotors mit der anzahl der Schritte addiert.
  Ist die Schrittmotorposition grrößer oder kleiner als 2048 oder -2048, wird diese auf null gesetzt. 
  Dies ist für die Berechnung des Drehwinkels und des Azimut wichtig, da sonst, zum Beispiel Drehwinkel, die größer als 360grad sind, enstehen würden.
*/
void stepperUpdate() {
  int inS1 = analogRead(LS1);  //Es wird eine neue Integer Variable namens inS1 erstellt, die den Wert, vom ersten Lichtsensor zugewiesen bekommt.
  int inS2 = analogRead(LS2);  //Es wird eine neue Integer Variable namens inS2 erstellt, die den Wert, vom zweiten Lichtsensor zugewiesen bekommt.
  int res = inS1 - inS2;  //Es wird eine neue Integer Variable namesns res erstellt, disese bekommt als Wert, 
                          //das Ergebnis der Differenz, der beiden Variablen inS1 und inS2, zugewiesen.
  
  if(!(res <= 10 && res >= -10)) {  //Wenn die Variable res größer als 10 und größer als -10 ist, wir die Bedingung der If-Schleife erfüllt.
    if(res > 0) { //Ist die Variable res größer als 0, wird der Schrittmotor um 8 Schritte im Uhrzeigersinn gedreht und die Variable motorPosition um acht erhöht.
      stepper.step(8);  //Schrittmotor bewegt sich acht Schritte in Richtung des Uhrzeigersinns.
      motorPosition += 8; //Variable wird um 8 erhöht
    } else {            //Ist die Variable kleiner als 0, wird der Schrittmotor um 8 Schritte gegen den Uhrzeigersinn gedreht und die Variable motorPosition um acht gesenkt.
      stepper.step(-8); //Schrittmotor bewegt sich acht Schritte gegen den Uhrzeigersinn.
      motorPosition -= 8; //Variable wird um 8 gesenkt.
    }
    stillStand = false; //Variable bekommt den Wert 0/false zugewiesen.
  } else {
    stillStand = true;  //Variable bekommt den Wert 1/true zugewiesen.
  }

  if(motorPosition >= 2048 || motorPosition >= -2048) { //Ist die Variable motorPostition größer als 2048 oder -2048, bekommt sie den Wert null zugewiesen.
    motorPosition = 0;  //Variable wird auf null gesetzt.
  }
}

//===========================================//
                //Display
//===========================================//
//===============================//
      //Spannungs Messung
//===============================//
/*
  Diese Funktion ließt den analogen Wert am Pin, der in der Variable pin gespeicher ist, multipliziert disen mit der Referenzspannung und
  teilt anschließend das Ergebnis durch den maximalen Wert, den ein Arduino an einem Analogen Pin messen kann. Dieser Wert ist hier 1024.
  Zum Schluss wird das Gesamterbenis der Rechnung in der Fließzahlvariable voltage gespeichert und mit dem return Schlüsselwort zurückgegeben.
*/
float readVoltage(int pin) {  //Integer Variable pin muss an die Funktion übergeben werden.
  #define refVoltage 5.0
  #define pinSteps 1024.0    //Definiert den maximalen Wert, der an einem Analogen Pin anliegen kann.
  float voltage = analogRead(pin) * refVoltage / pinSteps; //neue float Variable namens voltage wird erstellt und bekommt den oben beschreibenen Wert zugeteilt.
  return voltage; //die Variable voltage wird zurückgegeben.
}

//===============================//
          //Anzeige
//===============================//
/*
  Diese Funktion kümmert sich um das Design und das Anzeigen der Informationen, wie der Spannung, Drehwinkel und des Azimut. Dazu wird der Funktion zwei Variablen übergeben.
  Einmal eine Variable namens slide, des typs byte. Je nach Wert der Variable, wird eine Andere Information auf dem Display angegeben. 
  Die Variable hat den Typ byte, da dieser Werte bis 128 speichern kann und diese Menge genügent für die Funktion ist. Zudem wird dadurch Speicherplatz gespart, der bei dem Arduino begrenz ist.
  Die zweite Variable, die übergeben werden kann, lautet voltage. Diese ist eine Fließkommavariable(float) soll die Spannung des Photovoltaikmoduls speichern.
  Danach wird das Display menü gezeichnet. Dabei werden als erstes die drei Rechtecke gezeichnet und danach die gestrichelte Linie unter diesen. 
  Zum Schluss wird, je nach Wert der Variable slide, ein Passendes Symbol und die Information auf das Display geschrieben. Ist der Wert ungültig wird "Error" auf das Dispkkay geschrieben.
*/
void Slide(byte slide, float voltage) {
      display.clearDisplay();  //Display wird Gelöscht
      int PosWinkel = map(motorPosition, 0, 2048, 0, 360);  //Die Werte der Variable motorPostion, die von 0 bis 2048 reichen können, werden so runterskaliert, dass diese zwischen 0 und 360 liegen.
                                                              //Diese herunterskalierten Werte werden dann in der interger Variable PosWinkel gespiechert.
      int NegWinkel = map(motorPosition, 0, -2048, 0, 360)  //Die Werte der Variable motorPostion, die von 0 bis -2048 reichen können, werden so runterskaliert, dass diese zwischen 0 und 360 liegen.
                                                            //Diese herunterskalierten Werte werden dann in der interger Variable NegWinkel gespiechert.

      display.drawLine(0, 0, 5, 0, 1);    //Es werden drei Linien gezeichnet, die zusammen ein Teil
      display.drawLine(0, 56, 5, 56, 1);  //von einem Rechteck ergeben.
      display.drawLine(5, 0, 5, 56, 1);   //

      display.drawRect(10, 0, 108, 57, 1);  //Das mittlere große Rechteck wird gezeichnet, 
                                            //in dem die Informationen abgebildet sind.
      display.drawLine(128, 0, 124, 0, 1);    //Nun wird das rechte Teilrechteck gezeichnet,
      display.drawLine(128, 56, 124, 56, 1);  //dass wieder aus drei Linien besteht.
      display.drawLine(124, 0, 124, 56, 1);   //
      
      for(int x = 3; x < 128; x+=6) {          //Diese For-Schleife zeichnet die Strichellinie
        display.drawLine(x, 61, x+3, 61, 1);   //unter den Rechtecken, indem es alle drei Pixel eine 
      }                                        //drei Pixel lange Linie zeichnet.

      /*
        Diese If- und Else-If-Schleifen schreiben je nach Wert der Variable slide, verschiedene Informationen auf das Display.
        Ist diese Variable, zum Beispiel eins, wird die Position der Schrollbar verändert, ein Blitzsymbol wird gezeichnet
        und die dafür stehende Spannung des Photovoltaikmodules.
      */
  
      if(slide == 1) { //wenn die integer Variable slide gleich eins ist, wird die Bedingung der If-Schleife erfüllt.                   
        display.fillRect(2, 59, 31, 3, 1);  //Schrollbar wird gezeichnet

        display.drawBitmap(12, 10, menu_icon_blitz, 32, 32, 1);  //Blitzsymbol wird auf das Display gezeichnet.
        display.setTextSize(2);  //Textgröße des Displays wird auf 2 gesetz.
        display.setTextColor(1);  //Textfarbe des Displays wird auf 1/weiß gesetzt.
        display.setCursor(45, 18);  //Position des Textes wird auf x=45 und y=18 gesetzt.
        display.print(voltage);  //der Wert der float Variable voltage wird auf das Display geschrieben
        display.println("V");  //dahinter wird noch ein "V" für Volt geschreiben und es wird eine neue Zeile angefangen.
      } else if(slide == 2) { //Die Bedingung der If-Schleife ist erfüllt, wenn die Variable slide gleich eins ist.
        display.fillRect(33, 59, 31, 3, 1); //Scrollbar wird gezeichnet

        display.drawBitmap(12, 10, menu_icon_grad, 32, 32, 1);  //Drehwinkelsymbol
        display.setTextSize(2); //Textgröße des Displays wird auf 2 gesetzt.
        display.setTextColor(1);  //Textfarbge des Displays wird auf 1/weiß gesetzt.
        display.setCursor(55, 18);  //Posititon des Textes wird auf x=45 und y=18 gesetzt
        display.println(PosWinkel); //Der Wert der Variable mp wird auf das Display geschrieben
      } else if(slide == 3) { //Die Bedingung der If-Schleife wird erfüllt, wenn der Wert der Variable slide gleich drei ist.
        display.fillRect(64, 59, 31, 3, 1); //Scrollbar wird gezeichnet

        display.drawBitmap(15, 10, menu_icon_kompass, 32, 32, 1); //Kompasssymbol wird auf das Display gezeichnet
        display.setTextSize(2);   //Textgröße des Displays wird auf 2 gesetz.
        display.setTextColor(1);  //Textfarbe des Displays wird auf 1/weiß gesetzt.
        display.setCursor(60, 18);  //Position des Textes wird auf x=60 und y=18 gesetzt
        
        
        /*      
          Die folgende zwei If- und Else-If-Schleifen Abfolge, überprüft, auf welchem Drehwinkel sich das Photovoltaikmodul befindet und gibt dann den passenden Azimut an.
        */
         
        if(PosWinkel >= 0 && PosWinkel < 22.5) {
          display.println("N");
        } else if(PosWinkel >= 22.5 && PosWinkel < 67.5) {
          display.println("NO");
        } else if(PosWinkel >= 67.5 && PosWinkel < 112.5) {
          display.println("O");
        } else if(PosWinkel >= 112.5 && PosWinkel < 157.5) {
          display.println("SO");
        } else if(PosWinkel >= 157.5 && PosWinkel < 202.5) {
          display.println("S");
        } else if(PosWinkel >= 202.5 && PosWinkel < 247.5) {
          display.println("SW");
        } else if(PosWinkel >= 247.5 && PosWinkel < 292.5) {
          display.println("W");
        } else if(PosWinkel >= 292.5 && PosWinkel < 237.5) {
          display.println("NW");
        } else if(PosWinkel >= 237.5 && PosWinkel < 360) {
          display.println("N");
        }

        if(NegWinkel >= 0 && z < 22.5) {
          display.println("N");
        } else if(NegWinkel >= 22.5 && NegWinkel < 67.5) {
          display.println("NW");
        } else if(NegWinkel >= 67.5 && NegWinkel < 112.5) {
          display.println("W");
        } else if(NegWinkel >= 112.5 && NegWinkel < 157.5) {
          display.println("SW");
        } else if(NegWinkel >= 157.5 && NegWinkel < 202.5) {
          display.println("S");
        } else if(NegWinkel >= 202.5 && NegWinkel < 247.5) {
          display.println("SO");
        } else if(NegWinkel >= 247.5 && NegWinkel < 292.5) {
          display.println("O");
        } else if(NegWinkel >= 292.5 && NegWinkel < 237.5) {
          display.println("SO");
        } else if(NegWinkel >= 237.5 && NegWinkel < 360) {
          display.println("N");
        }

      } else if(slide == 4) { //Die Bedingung der If-Schleife ist erfüllt, wenn der Wert der Variable slide gleich vier ist.
        display.fillRect(95, 59, 31, 3, 1); //Die Scrollbar wird gezeichnet.
        //TODO
      } else {  //Wenn keine der vorherigen If-Schleifen ausgeführt sind, wird die else-Schleife ausgeführt.
        display.clearDisplay(); //Display wird geleert
        display.setTextSize(3); //Schriftgröße wird auf drei gesetzt.
        display.setCursor(22, 30);  //Die Postition des Textes wird auf x=22 und y=30 gesetzt.
        display.println("Error"); //Auf dem Display wird der Text "Error" angezeigt.
      }
          
      display.display();  //Display wird aktuallisiert
}

//===============================//
        //Display Update
//===============================//
/*
  In dieser Funktion wird die Variable "sldAnzL" alle 2,5 Sekunden um eins erhöht, bis sie den Wert drei erreicht. Ist dies der Fall, wird die Variable auf 0 zurückgesetzt.
  Sollte die boolsiche Variable stillStand den Wert 0/false besitzen, wird die Funktion alle 2,5 Sekunden ausgeführt. 
  Ist dies nicht der Fall und die Variable besitzt den Wert 1/true, wird die Funktion so schnell ausgeführt, wie der Arduino kann. 
  Dies ist nötig, da wenn sich der Schrittmotor drehen würde und gleichzeitig mehrmals die Funktion Slide ausgeführt werden sollte, 
  der Arduino zu viele Aufgaben hat, und so den Schrittmotor langsamer drehen lassen würde und das Menü auf dem Display sich nicht mehr Richtig updatet.
  Ich habe in dieser bewusst keine delay Funktion benutzt, da sonst, sich der Schrittmorot nur noch alle 2,5 Sekunden drehen lassen würde.
  Deshalb habe ich ein millis delay verwendet, sodass sich der Schrittmotor auch dann bewegen kann, wenn der Arduino 2,5 sekunden für einen Menupunktwechsel warten muss.
  Ich habe auch im restlichen Code keine einzige delay Funktion benutzt, sodass ich auch dann Code ausführen kann, wenn der Arduino für eine bestimmte Zeit wartet.
  Dies wäre mit delay Funktion nicht möglich.
*/
void updateDisplay() {
  if((millis() - sldAnzL) >= 2500) {  //Wenn 2,5 sekunden gewartet worden sind, wird die If-Schleife ausgeführt.
    slide >= 3 ? slide = 0 : false; //Wenn die Variable slide größer oder gleich 3 ist, wird diese auf Null gesetzt.
    slide++;  //Variable wird um eins erhöht.
    sldAnzL = millis(); //der 2,5 sekunden timer wird zurückgesetzt.
    if(stillStand == false) { //Hat die Variable stillStand den Wert 0/false, wird die Funktion Slide alle 2,5 sekunden ausgeführt.
      Slide(slide, readVoltage(PVMod)); //Der Funktion Slide, werden die Werte der Variable slide und der Funktion readVoltage() übergeben.
    }
  }

  if(stillStand == true) {  //Hat die Variable stillStand den Wert 1/true, wrid die Funktion slide ausgefürht.
    Slide(slide, readVoltage(PVMod)); //Der Funktion Slide, werden die Werte der Variable slide und der Funktion readVoltage() übergeben.    
  }
}