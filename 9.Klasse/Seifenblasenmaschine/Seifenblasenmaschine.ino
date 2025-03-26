#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Schrittmotor
#include <Stepper.h>

#define In1 4
#define In2 5
#define In3 6
#define In4 7

int SPU = 2048; // Schritte pro Umdrehung.
Stepper Motor(SPU, In4,In2,In3,In1); // Der Schrittmotor erhält die Bezeichnung "Motor" und es wird angegeben an welchen Pins der Motor angeschlossen ist.


//Gleichschrittmotor
#define GSM1 10
#define GSM1_In1 8
#define GSM1_In2 9
int GSM1_Geschwindigkeit = 255;

//=====================================================================================================

//Steuerung
#define B1 2  //Button 1 / Rechter Button
#define B2 3  //Button 2 / Linker Button
#define Potenziometer A3  //Potenziometer / Mitte
#define maxwert 1023  // Der Maximalwert von dem Potenziometer
#define minwert 0 //Der Minimalwert von dem Potenziometer

const unsigned int Prelltime = 100;     //prellzeit für die Buttons

//Loadingscreen - Variable
bool skip_loading_screen = false;       //wenn die boolische Variable auf true gesetzt ist, wird der ladescreen übersprungen

// Menu - Variablen
unsigned int main_menu_variable = 1;
unsigned int On_Off_menu_variable = 1;
unsigned int untermenu_2_variable = 1;

//An/Aus - Variablen
bool On = false;    //Wenn die boolische Variable auf true gesetzt ist, wird das Unterprogramm an() ausgeführt
bool Off = true;    //Wenn die boolische Variable auf true gesetzt ist, wird das Unterprogramm aus() ausgeführt
bool off_anfrage = false;
bool off_bestaetigung = false;

//delay - Variablen
unsigned long millis1;    
unsigned long millis2;    
unsigned long millis3;          
unsigned long millis5;    
unsigned long millis6;   
unsigned long millis7;   

//andere Variablen
unsigned int Ablauf_Variable = 1;
int Balken_breite;

//=================================================================================//
//                                    Setup                                        //
//=================================================================================//
void setup()
{ 
  //Serieller Monitor 
  Serial.begin(9600); 

  //Steuerung
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(Potenziometer, INPUT);

  //Gleichschrittmotor
  pinMode(GSM1, OUTPUT);
  pinMode(GSM1_In1, OUTPUT);
  pinMode(GSM1_In2, OUTPUT);

  //OLED Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))    
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);          
  }

  delay(2000); 

  //Ladescreen
  if(skip_loading_screen == false)    //Wenn die Variable auf false ist, wird die geschweifte Klammer ausgeführt
  {
    for(int i = 0; i<=118; i=i+2)   //Die Variable i wird auf 0 gesetzt und wird dann solage mit 2 addiert bis sie den Wert 118 erreicht hat
    {   
      display.clearDisplay();       
      display.setTextSize(2);       
      display.setTextColor(WHITE);  
      display.setCursor(45, 15);   
      display.println("O_O");
      
      display.setCursor(42, 10);
      display.drawRect(5, 40, 119, 10, 1);
      display.fillRect(5, 40, i, 10, 1);
      display.display();  //das Display wird aktualisiert
    }
  } 
  else //Ist die Variable skip_loading_schreen nicht auf false wird der ladebalken nicht angezeigt
  {
    display.clearDisplay();       
    display.setTextSize(2);       
    display.setTextColor(WHITE);
    display.println("");
    display.display();  //das Display wird aktualisiert
  }  
  //Schrittmotor
  Motor.setSpeed(5); //Die Motorgeschwindigkeit wird auf 5 gesetzt
  Serial.println("\t Setup");
}

//=================================================================================//
//                                    Loop                                         //
//=================================================================================//
void loop()
{

  main_menu_ueberpruefung();  //das unterprogramm main_menu_ueberpruefung wird ausgeführt
  main_menu();    //das unterprogramm main_menu wird ausgeführt

  if(On == true)  //wenn die Variable On den wert 1(true) hat, wird das unterprogramm on ausgeführt
  {
    on();
  }
  else if(Off == true)  //wenn die Variable On nicht den wert 1(true) hat und die Variable Off den Wert 1(true) hat , wird die geschweifte klammer ausgeführt und so mit auch das off unterprogramm
  {
    off();
  }
  Serial.println("\t Loop");
}



//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                                                                                                            //
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//



//=================================================================================//
//                               Main Menu                                         //
//=================================================================================//
/*
  Das Unterprogramm main_menu zeigt auf den Display das Hauptmenü an, welches aus zwei Menüpunkten besteht, wobei der erste Menüpunkt "On/Off" heißt und der zweite "Settings".
  Das Hauptmenü funktioniert wie alle anderen Menüs in dem sketch, es gibt eine Variable(hier main_menu_variable) für jedes Menü, je nach Wert der Variable zeigt das Menü einen anderen Menüpunkt an,
  die Variable wird über ein Potentiometer verändert und wird dann mit eine Switch/Case Anweisung untersucht, hier gibt es zwei Cases, der erste case(case 1) zeigt das gleiche an wie case 2, nur mit dem Utnerschied
  das bei case 1 beim ersten menüpunkt ein Rechteck den Text umgibt und beim zweiten case, der zweite Menüpuznkt mit einem Rechteck umgeben ist.  
*/
void main_menu()
{
  switch(main_menu_variable)  //die Variable main_menu_variable wird untersucht
  {
    case 1:   //wenn diese den wert 1 hat, wird der erste menüpunkt auf das display geschreiben und der ersten Punkt ausgewählt
      display.clearDisplay(); //Display wird gelöscht
      display.drawRect(2, 1, 124, 30, 1); //ein rechteck wird an der x stelle 2 und an der y stelle 1 angezeigt
      display.setCursor(5, 8);  
      display.println("On/Off"); //der Text "On/Off" wird an der stelle 5, 8 am Display angezeigt 
    
    //display.drawRect(2, 33, 124, 30, 1);    //Ausgeklammert
      display.setCursor(5, 40);
      display.println("Settings"); //Der Text "Settings" wird aan der stelle 5, 40 am Display angezeigt
      display.display();  //das Display wird aktualisiert
    break;
    case 2: //wenn diese den wert 2 hat, wird der 1 menüpunkt auf das display geschreiben und der zweite Punkt ausgewählt
      display.clearDisplay(); //Display wird geslöscht
    //display.drawRect(2, 1, 124, 30, 1);     //Ausgeklammert
      display.setCursor(5, 8);
      display.println("On/Off");  //der Text "On/Off" wird an der stelle 5, 8 am Display angezeigt 
    
      display.drawRect(2, 33, 124, 30, 1);  //ein rechteck wird an der x stelle 2 und an der y stelle 33 angezeigt
      display.setCursor(5, 40);
      display.println("Settings"); //Der Text "Settings" wird aan der stelle 5, 40 am Display angezeigt
      display.display();  //das Display wird aktualisiert
    break;
  }
}

//=================================================================================//
//                          Main Menu Ueberpruefung                                //
//=================================================================================//
/*
  Das Unterprogramm main_menu_ueberpruefung sorgt dafür, dass man mit dem Potenziomter zwischen den Menüpunkten navigieren kann und das wenn man auf den rechten Button(B1) drückt das Unterprogramm main_menu_interaktion ausgeführt wird
  Das geht mit drei if-Schleifen, die den Zustand vom Potenziometer / Button auslesen und dem entsprächend den Wert der Variable main_menu_variable ändern oder das Unterprogramm main_menu_interaktion ausführt.
*/
void main_menu_ueberpruefung()
{
  if(analogRead(Potenziometer) >=0 && analogRead(Potenziometer) <=510)    //wenn der Wert vom Potenziometer größer als 0 und kleiner als 510 ist, wird die geschweifte Klammer ausgeführt
  {
    main_menu_variable = 1; //die Variable wird auf den Wert 1 gesetzt
  }
  else if(analogRead(Potenziometer) >=510 && analogRead(Potenziometer) <=1023)  //wenn der Wert vom Potenziometer größer als 510 und kleiner als 1023 ist, wird die geschweifte Klammer ausgeführt
  {
    main_menu_variable = 2; //die Variable wird auf den Wert 2 gesetzt
  }
 
  if(!digitalRead(B1))  //Wenn der erste Button gedrückt wird, wird das Unterprogramm main_menu_interaktion ausgeführt
  {
    main_menu_interaktion();
  }
}

//=================================================================================//
//                           Main Menu Interaktion                                 //
//=================================================================================//
/*
  Das Unterprogramm main_menu_interaktion sorgt dafür, das man mit den Menüpunkten interagiren kann, also das wenn man den rechten Button(B1) drückt auch etwas passiert,
  z.B. man geht in ein Untermenü oder man startet die Seifenblasenmaschine. Das wird auch mit einer Switch/Case Anweisen überprüft. Wenn man den Rechten Butten(B1) drückt, wird dieses Unterprogramm(main_menu_interaktiom)
  ausgeführt und somit auch die Switch/Case Anweisung, diese untersucht wieder die main_menu_variable und führt je nach dem welchen Menüpunkt man ausgewählt hat den entsprechenden Case(case 1 oder case 2) aus,
  damit kommt man dann in eine weiteres Unterprogramm(hier untermenu_1 oder untermenu_2).
*/
void main_menu_interaktion()
{
  switch(main_menu_variable)  //die Variable main_menu_variable wird Untersucht
  {
    case 1: //wenn die Variable den Wert 1 hat, wird das Unterprogramm untermenu_1 ausgeführt
      untermenu_1();
    break;
    case 2: //wenn die Variable den Wert 2 hat, wird das Unterprgramm untermenu_2 ausgeführt
      untermenu_2();
    break;
  }
}



//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                                                                                                            //
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//



//=================================================================================//
//                                 Untermenu 1                                     //
//=================================================================================//
/*
  Das erste Untermenu macht im grunde das Gleiche wie die anderen Untermenues, es setzt zuerst die delay-Variablen auf den Wert des Befehles millis, dann kommt immer zuerst die while-Schleife die solange ausgeführt wird,
  bis man den zurück Button(B2) drückt, in dieser while schleife sind dann immer zwei if-Schleifen, die dafür sorgen, dass wenn die Seifenblasenmaschine an ist, das Unterprogramm "an" ausgeführt wird oder wenn die Seifenblasenmaschine
  aus ist, das Unterprogramm "aus" ausführt.
*/
void untermenu_1()
{
  millis2 = millis(); //die Variable millis 2 wird auf den wert des Befehles millis gesetzt
  
  while(digitalRead(B2))  //solange der zurück Button(B2) nicht gedrückt wird, wird die geschweifte Klammer ausgeführt
  {
    if(On == true)  //wenn die Variable den Wert 1(true) hat, wird das Unterprogramm on ausgeführt
    {
      on();
    }
    else if(Off == true) //wenn die Variable den Wert 1(true) hat, wird das Unterprogramm off ausgeführt 
    {
      off();
    }
    Serial.println("\t Untermenu_1");
    On_Off_menu();  //das Unterprogramm On_Off_menu wird ausgeführt
    On_Off_ueberpruefung(); //das Unterprogramm On_Off_ueberpruefung wird ausgeführt
  }

  while(!digitalRead(B2)) {}  //Wenn der Zurück Button(B2) gedrückt wird, wird die Geschweifte Klammer ausgeführt und somit, wird auch das Unterprogramm verlassen
}

//=================================================================================//
//                          On_Off- / Untermenu1- Menu                             //
//=================================================================================//
void On_Off_menu()
{
  switch(On_Off_menu_variable)
  {
    case 1:   //Wenn die Variable den Wert 1 hat, wird der 1 menupunkt aufs´s Display geschrieben und der erste Punkt wird ausgewählt
    display.clearDisplay(); //display Wird gelöscht
    display.drawRect(2, 1, 124, 30, 1); //ein Rechteck wird an der stelle 2,1 auf dem Display angezeigt
    display.setCursor(5, 8);  
    display.println("Aus");  //der Text "Aus" wird auf dem Display an der Stelle 5,8 angezeigt
    
  //display.drawRect(2, 33, 124, 30, 1);    //Auskommentiert
    display.setCursor(5, 40);
    display.println("An");  //der Text "An" wird auf das Display an der Stelle 5, 40 angezeigt
    display.display();  //das Display wird aktualisiert

    break;
    case 2:   //Wenn die Variable den Wert 1 hat, wird der 2 menupunkt aufs´s Display geschrieben und der zweite Punkt wird ausgewählt
    display.clearDisplay(); //das Display wird gelöscht
  //display.drawRect(2, 1, 124, 30, 1);
    display.setCursor(5, 8);
    display.println("Aus"); //der Text "Aus" wird an der Stelle 5, 8 angezeigt
    
    display.drawRect(2, 33, 124, 30, 1);  //ein Rechteck wird auf dem Display am der Stelle 2, 33 angezeigt
    display.setCursor(5, 40); 
    display.println("An");  //der Text "An" wird an der Stelle 5, 40 auf dem Display angezeigt
    display.display();  //das Display wird aktualisiert
    break;
  }
}

//=================================================================================//
//                       On_Off- / Untermenu1- Ueberpruefung                       //
//=================================================================================//
void On_Off_ueberpruefung()
{
  if(analogRead(Potenziometer) >=0 && analogRead(Potenziometer) <=510)  //wenn der Wert vom Potenziometer größer als 0 und kleiner als 510 ist, wird die geschweifte Klammer ausgeführt
  {
    On_Off_menu_variable = 1; //die Variable wird auf den Wert 1 gesetzt
    
  }
  else if(analogRead(Potenziometer) >=510 && analogRead(Potenziometer) <=1023)  //wenn der Wert vom Potenziometer größer als 510 und kleiner als 1023 ist, wird die geschweifte Klammer ausgeführt
  {
    On_Off_menu_variable = 2; //die Variable wird auf den Wert 2 gesetzt
  }
 
  if(!digitalRead(B1))  //wenn der rechte Button(B1) gedrückt wird, wird das Unterprogramm On_Off_interaktion ausgeführt
  {
    On_Off_interaktion();
  }
}

//=================================================================================//
//                         On_Off- / Untermenu1- Interaktion                       //
//=================================================================================//
void On_Off_interaktion()
{
  switch(On_Off_menu_variable)  //Die Variable On_Off_mnenu_variable wird untersucht
  {
    case 1: //wenn die Variable den wert 1 hat, wird die Variable off_anfrage auf 1(true) gesetzt
      off_anfrage = true;
    break; //hier ender Case 1
    case 2: //wenn die Variable den wert 2 hat wird Case 2 ausgeführt
      Off = false;  //Variable wird auf 0(false) gesetzt
      On = true;  //Variable wird auf 1(true) gesetzt
      millis5 = millis(); //die Variable millis5 wird auf den Wert des Befehles millis() gesetzt
      off_anfrage = false; //Variable wird auf 0(false) gesetzt
    break; //hier endet Case 2
  }
}



//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                                                                                                            //
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//



//=================================================================================//
//                               Untermenu 2                                       //
//=================================================================================//
void untermenu_2()
{
  while(digitalRead(B2)==1) //Wenn der zurück Button(B2) nicht Gedrückt wird, wir die geschweifte Klammer ausgeführt
  {
    if(On == true)  //Wenn die Variable den Wert 1(true) hat, wird das Unterprogramm on ausgeführt
    {
      on();
    }
    else if(Off == true)  //Wenn die Variable den Wert 1(true) hat, wird das Unterprogramm off ausgeführt
    {
      off();
    }
    Serial.println("\t Untermenu_2");
    Untermenu_2_ueberpruefung();  //das Unterprogramm Untermenu_2_ueberpruefung wird ausgeführt
    Untermenu_2_Menu(); //das Unterprogramm Untermenu_2_Menu wird ausgeführt
  }

  while(digitalRead(B2) == 0) {}  //wenn der zurück Button(B2) gedrückt wird, wird die leere geschweifte Klammer ausgeführt und das Unterprogramm wird verlassen
}

//=================================================================================//
//                            Untermenu 2 Menu                                     //
//=================================================================================//
void Untermenu_2_Menu()
{
  switch(untermenu_2_variable)  //die Variable untermenu_2_variable wird Untersucht
  {
    case 1: //wenn diese den wert 1 hat, wird der erste menüpunkt des zweiten untermenues auf dem Display angezeigt
      display.clearDisplay(); //das Display wird gelöscht
      display.drawRect(2, 1, 124, 30, 1); //ein Rechteck wird an der Position 2, 1 auf dem Display angezeigt
      display.setCursor(5, 8);
      display.println("Anzahl");  //der Text "Anzahl" wird an der Stelle 5, 8 auf dem Display angezeigt
    
    //display.drawRect(2, 33, 124, 30, 1);
      display.setCursor(5, 40);
      display.println("Empty"); //der Text "Empty" wird an der Stelle 5, 40 auf dem Display angezeigt
      display.display();  //das Display wird aktualisiert
    break;  //hier endet case 1
    case 2: //wenn diese den wert 1 hat, wird der zweite menüpunkt des zweiten untermenues auf dem Display angezeigt
      display.clearDisplay();
    //display.drawRect(2, 1, 124, 30, 1);
      display.setCursor(5, 8);
      display.println("Anzahl");  //der Text "Anzahl" wird an der Stelle 5, 8 auf dem Display angezeigt
    
      display.drawRect(2, 33, 124, 30, 1);  //ein Rechteck wird an der Stelle 5, 33 auf das Display gezeichnet
      display.setCursor(5, 40); 
      display.println("Empty"); //der Text "Empty" wird an der Stelle 5, 40 auf dem Dispaly angezeigt
      display.display();  //das Display wird aktualisiert
    break;  //hier endet case 2
  }
}

//=================================================================================//
//                          Untermenu 2 Ueberpruefung                              //
//=================================================================================//
void Untermenu_2_ueberpruefung()
{
  if(analogRead(Potenziometer) >=0 && analogRead(Potenziometer) <=510)   //wenn der Wert vom Potenziometer größer als 0 und kleiner als 510 ist, wird die geschweifte Klammer ausgeführt
  {
    untermenu_2_variable = 1; //die Variable wird auf den Wert 1 gesetzt
  }
  
  if(analogRead(Potenziometer) >=510 && analogRead(Potenziometer) <=1023) //wenn der Wert vom Potenziometer größer als 510 und kleiner als 1023 ist, wird die geschweifte Klammer ausgeführt
  {
    untermenu_2_variable = 2; //die Variable wird auf den Wert 2 gesetzt
  }
 
  if((millis() - millis3) >=Prelltime)  //delay von 100 millisekunden(ms) in der millisschreibweise
  { 
    millis3 = millis(); //die Variable millis3 wird auf den Wert des Befehles millis gesetzt
    if(!digitalRead(B1))  //wenn der rechte Button(B1) gedrückt wird, wird die geschweifte Klammer ausgeführt 
    {
      Untermenu_2_interaktion();  //das Unterprogramm Untermenu_2_interaktion wird ausgeführt
    }
  }
}

//=================================================================================//
//                            Untermenu 2 Interaktion                              //
//=================================================================================//
void Untermenu_2_interaktion() 
{
  switch(untermenu_2_variable)  //die Variable untermenu_2_variable wird untersucht
  {
    case 1: //wenn die Variable den wert 1 hat, wird das Unterprogramm Anzahl ausgeführt
      Untermenu1_von_Untermenu_2();
    break; //hier endet case 1
    case 2: //wenn die Variable den wert 2 hat, wird nichts ausgeführt
      
    break; //hier endet case 2
  }  
}



//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                                                                                                            //
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//



//=================================================================================//
//                          Untermenu1_von_Untermenu_2                             //
//=================================================================================//
void Untermenu1_von_Untermenu_2()
{ 
  while(digitalRead(B2) == 1) //wenn der zurück Button(B2) nicht wird, wird die geschweifte Klammer ausgeführt
  {
    if(On == true)  //wenn die Variable den Wert 1(true) hat, wird das Unterprogramm on ausgeführt
    {
      on();
    }
    else if(Off == true)  //wenn die Variable den Wert 1(true) hat, wird das Unterprogramm on ausgeführt
    {
      off();
    }
      
    anzahl_steuerung(); //das Unterprogramm balken_steuerung ausgeführt
    anzahl_menu();  //das Unterprogramm balken_menu wird ausgeführt
    }

  while(digitalRead(B2) == 0) {}  //wenn der zurück Button(B2) gedrückt wird, wird die leere geschweifte Klammer ausgeführt und verlässt somit auch das Unterprogramm 
}

//=================================================================================//
//                                      Anzahl                                     //
//=================================================================================//
void anzahl_menu()
{ 
  display.clearDisplay(); //das Display wird gelöscht
  display.setCursor(10,10); 
  display.println("Anzahl:"); //der Text "Anzahl:" wird an der Stelle 10, 10 auf dem Display angezeigt
  display.drawRect(10, 35, 108, 15, 1); //ein rechteck wird auf das Display an der Stelle 10, 35 gezeichnet
  display.fillRect(10, 35, Balken_breite, 15, 1); //das selbe rechteck wird oben wird auf das Display gezeichnet, nur das dieses Rechteck ausgefühlt hat und sich dort die Breite ändert

  display.display();  //das Display wird aktualisiert
}

void anzahl_steuerung()
{
  if((analogRead(Potenziometer) >= 0) && (analogRead(Potenziometer) <= 354))   //wenn der Wert vom Potenziometer größer als 0 und kleiner als 354 ist, wird die geschweifte Klammer ausgeführt
  {
    Balken_breite = 36; //die Variable wird auf den Wert 36 gesetzt
    GSM1_Geschwindigkeit = 85;  //die Variable wird auf den Wert 85 gesetzt
  }
  
  if((analogRead(Potenziometer) >= 354) && (analogRead(Potenziometer) <= 708))   //wenn der Wert vom Potenziometer größer als 354 und kleiner als 708 ist, wird die geschweifte Klammer ausgeführt
  {
    Balken_breite = 72; //die Variable wird auf den Wert 72 gesetzt
    GSM1_Geschwindigkeit = 170; //die Variable wird auf den Wert 170 gesetzt
  }
  
  if((analogRead(Potenziometer) >= 708) && (analogRead(Potenziometer) <= 1023))  //wenn der Wert vom Potenziometer größer als 708 und kleiner als 1023 ist, wird die geschweifte Klammer ausgeführt
  {
    Balken_breite = 108;  //die Variable wird auf den Wert 108 gesetzt
    GSM1_Geschwindigkeit = 255; //die Variable wird auf den Wert 255 gesetzt
  }
}





//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                                                                                                                            //
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//



//=================================================================================//
//                                    On                                           //
//=================================================================================//
  void on()
  {    
    //hier kommt der Code rein, wenn die Seifenblasenmaschine angeschaltet ist

    if(Ablauf_Variable == 1)  //wenn die Variable Ablauf_Variable den Wert 1 hat, wird die geschweifte Klamer ausgeführt
    { 
      On = true;  //die Variable wird auf 1(true) gesetzt
      Off = false;  //die Variable wird auf 0(false) gesetzt
      
      Motor.step(1024); //der Schrittmotor macht 1024 schritte in positive Richtung
      if((millis() - millis5) >= 2500)  //es wird 2,5 sekunden(2500ms) gewartet (in millisschreibweise)
      { 
        Ablauf_Variable = 2;  //die Variable wird auf den Wert 2 gesetzt, somit wird die if schleife in Zeile 458 nicht mehr ausgeführt
        millis6 = millis(); //die Variable millis6 wird auf den Wert des Befehles millis gesetzt
      }
    }

    if(Ablauf_Variable == 2)  //wenn die Variable Ablauf_Variable den Wert 2 hat, wird die geschweifte Klamer ausgeführt
    { 
      Motor.step(-1024);  //der Schrittmotor macht 1024 schritte in negative Richtung

      if((millis() - millis6) >= 2500)  //es wird 2,5 sekunden(2500ms) gewartet (in millisschreibweise)
      {
        Ablauf_Variable = 3;  //die Variable wird auf den Wert 2 gesetzt, somit wird die if schleife in Zeile 471 nicht mehr ausgeführt
        millis7 = millis(); //die Variable millis7 wird auf den Wert des Befehles millis gesetzt
      }
    }

    if(Ablauf_Variable == 3)  //wenn die Variable Ablauf_Variable den Wert 3 hat, wird die geschweifte Klamer ausgeführt
    {
      digitalWrite(GSM1_In1, LOW);  //In1 des Gleichschrittmotors wird disabled 
      digitalWrite(GSM1_In2, HIGH); //In2 des Gleichschrittmotors wird enabled
      analogWrite(GSM1,  GSM1_Geschwindigkeit); //die Geschwindigkeit von dem Gleichschrittmotor wird auf die Variable GSM1_Geschwindikeit festgelegt
      //Motor dreht sich
      
      if(((millis() - millis7) >= 5500) && (off_anfrage == false))  //die Geschweifte klammer wird erst ausgeführt wenn die Variable off_anfrage den Wert 0(false) hat und 5.5sekunden(5500ms) gewartet wurden 
      {
        Ablauf_Variable = 1;  //die Variable wird auf den Wert 1 gesetzt und somit wird wieder die if schleife in Zeile 458 ausgeführt ==> es entseht ein loop, das heißt die 3 ifschleifen werden nacheinander ausgeführt, bis man den off taster betätigt
        millis5 = millis(); //die Variable millis5 wird auf den Wert des Befehles millis gesetzt
        Off = false;  //die Variable wird auf den Wert 0(false) gesetzt
        On = true;  //die Variable wird auf den Wert 1(true) gesetzt

        digitalWrite(GSM1_In1, LOW);  //In1 des Gleichschrittmotors wird disabled 
        digitalWrite(GSM1_In2, LOW);  //In2 des Gleichschrittmotors wird disabled 
        analogWrite(GSM1, 0); //die Geschwindigkeit von dem Gleichschrittmotor wird auf 0 gesetzt

        //Motor stoppt

      } else if(((millis() - millis7) >= 5500) && (off_anfrage == true))  //die Geschweifte klammer wird erst ausgeführt wenn die Variable off_anfrage den Wert 1(true) hat und 5.5sekunden(5500ms) gewartet wurden 
        { 
          Off = true; //die Variable wird auf den Wert 1(true) gesetzt
          On = false; //die Variable wird auf den Wert 0(false) gesetzt
          off_bestaetigung = true;  //die Variable wird auf den Wert 1(true) gesetzt
          Ablauf_Variable = 1;  //die Variable wird auf den Wert 1 gesetzt
          millis5 = millis(); //die Variable millis5 wird auf den Wert des Befehles millis gesetzt
          off_anfrage = false;  //die Variable wird auf den Wert 0(false) gesetzt
        
          digitalWrite(GSM1_In1, LOW);  //In1 des Gleichschrittmotors wird disabled 
          digitalWrite(GSM1_In2, LOW);  //In2 des Gleichschrittmotors wird disabled 
          analogWrite(GSM1, 0); //die Geschwindigkeit von dem Gleichschrittmotor wird auf 0 gesetzt        
        }
    }
  }

//=================================================================================//
//                                   Off                                           //
//=================================================================================//
void off()
{
  //Hier kommt der Code rein, der Ausgeführt wird, wenn die Seifenblasenmaschine ausgeschaltet ist
}