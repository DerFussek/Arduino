#define led 13
#define akku 12
#define motor 11
#define solar A0

const float uWert = 0.3;
const float uGrenze = 0.7;

const float oGrenze = 2.5;
const float AREF = 5.0;

bool f_Akku = false;

void setup() {

  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(akku, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(solar, INPUT);
}

void loop() {

  float voltage = (analogRead(solar) / 1023.0) * AREF;

  Serial.flush();
  Serial.print("Spannung: ");
  Serial.println(voltage, 1);
  Serial.println();
  
  String input = "";
  input = Serial.readStringUntil('\n');

  if(voltage < uWert) {
    Serial.println("Warnung: Wenig el. Energie im Netz!");
    Serial.println("--> alle Großverbraucher werden abgeschaltet");
    Serial.println();

    digitalWrite(akku, LOW);
    f_Akku = false;

    digitalWrite(motor, LOW);
    
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
  }


  if(voltage > uWert && voltage < uGrenze) {
    Serial.println("Warnung: Wenig el. Energie im Netzt!");
    Serial.println("--> Kleinverbraucher bleiben im Nezt");
    Serial.println();

    digitalWrite(led, HIGH);
    digitalWrite(motor, LOW);

    digitalWrite(akku, LOW);
    f_Akku = false;
  }


  if(voltage >= uGrenze && voltage < oGrenze && !f_Akku) {
    digitalWrite(led, HIGH);

    Serial.println("Info: Es steht genug el. Energie zur verfügung");
    Serial.println("--> 'A' drücken um den Akku zu laden!");

    if(!f_Akku && input.equals("a")) {
      
      digitalWrite(akku, HIGH);
      f_Akku = true;
      Serial.println("Info: Akku wird jetzt geladen");
    } else if(f_Akku && input.equals("p")) {
      
      digitalWrite(akku, LOW);
      f_Akku = false;
      Serial.println("Info: Akku wird nichtmehr geladen");
    }
  }

  
  if(voltage >= 2.5) {
    Serial.println("Warnung: Netzspannung zu hoch!");
    Serial.println("--> keine Akkus aufladen");
    Serial.println("--> Spannung für Motor ausreichend!");

    digitalWrite(motor, HIGH);

    digitalWrite(led, LOW);
    digitalWrite(akku, LOW);
    f_Akku = false;

    delay(2000);
  }
}
