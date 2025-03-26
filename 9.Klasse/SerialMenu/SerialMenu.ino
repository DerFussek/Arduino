//Buzzer
#define Buzzer 8

//Keys
#define down 45
#define up 51
#define enter 49
#define back 47

//=====================================================================================
// |                                Variablen                                         |
//=====================================================================================

unsigned int menu = 1;

boolean updateSerial = true;

String command;


//=====================================================================================
// |                                  Setup                                           |
//=====================================================================================


void setup() 
{
  //Serial Monior
  Serial.begin(9600);
  

  //PinModes
  pinMode(down, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(enter, INPUT_PULLUP);
  pinMode(back, INPUT_PULLUP);

  ClearSerial();
}


//=====================================================================================
// |                                  Loop                                            |
//=====================================================================================


void loop() 
{ 
  if(Serial.available())
  {
    command = Serial.readString();
  
  }
  
  delay(250);
  
 
  
  if(command =="down")
  {
    updateSerial = true;
    menu++;
  }

  if(command =="up")
  {
    updateSerial = true;
    menu--;
  }

  if(command =="enter")
  {
     delay(50);
     interaction();
  }

  if(!digitalRead(enter))
  {
    interaction();
  }
  
  
  if(!digitalRead(down))
  {
    updateSerial = true;
    menu++;
    }

  if(!digitalRead(up))
  {
    updateSerial = true;
    menu--;
  }
   command = "not defined";
   Menu();
}


//=====================================================================================
// |                                  Menu                                            |
//=====================================================================================




void Menu() 
{
  switch(menu)
  {
    case 0:
      menu = 1;
    break;
    case 1:
      if(updateSerial == true)
      { 
        ClearSerial();
        
        Serial.println(F("============================"));
        Serial.print("-->"); Serial.println("Level 1");
        Serial.print("   "); Serial.println("Level 2");
        Serial.print("   "); Serial.println("Level 3");
        Serial.print("   "); Serial.println("Level 4");
        Serial.println(F("============================"));

        updateSerial = false;
      }
    break;
    case 2:
        if(updateSerial == true)
        {
          ClearSerial(); 
          
          Serial.println(F("============================"));
          Serial.print("   "); Serial.println("Level 1");
          Serial.print("-->"); Serial.println("Level 2");
          Serial.print("   "); Serial.println("Level 3");
          Serial.print("   "); Serial.println("Level 4");
          Serial.println(F("============================"));
      
          updateSerial = false;
        }
    break;
    case 3:
         
          
          
          if(updateSerial == true)
          {
            ClearSerial();
            
            Serial.println(F("============================"));
            Serial.print("   "); Serial.println("Level 1");
            Serial.print("   "); Serial.println("Level 2");
            Serial.print("-->"); Serial.println("Level 3");
            Serial.print("   "); Serial.println("Level 4");
            Serial.println(F("============================"));

        updateSerial = false;
      }
    break;
    case 4:
            
            
            
            if(updateSerial == true)
            {
              ClearSerial();   
              
              Serial.println(F("============================"));
              Serial.print("   "); Serial.println("Level 1");
              Serial.print("   "); Serial.println("Level 2");
              Serial.print("   "); Serial.println("Level 3");
              Serial.print("-->"); Serial.println("Level 4");
              Serial.println(F("============================"));
      
              updateSerial = false;
            }
    break;
    case 5:
      menu = 4; 
    break;
  
  
  }
  
}


//=====================================================================================
// |                                Interaktion_menu                                  |
//=====================================================================================


void interaction()
{
  switch(menu)
  {
    case 1:
      Level1_submenu_abfrage();
    break;
    case 2:
       Level2();
    break;
    case 3:
        Level3();
    break;
    case 4:
          Level4();
    break;
  }
}

//=====================================================================================
// |                                   Clear                                          |
//=====================================================================================




void ClearSerial()
{
  Serial.println(""); Serial.println(""); Serial.println("");
  Serial.println(""); Serial.println(""); Serial.println("");
  Serial.println(""); Serial.println(""); Serial.println("");
  Serial.println(""); Serial.println(""); Serial.println("");
  Serial.println(""); Serial.println(""); Serial.println("");
  Serial.println(""); Serial.println(""); Serial.println("");
} 