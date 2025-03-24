#include <Servo.h>

#define ServoPin 11
Servo servo = Servo();


void setup() {
  Serial.begin(9600);
  servo.attach(ServoPin);
  servo.write(0);
}

void loop() {
  servo.write(getInt());
}

int getInt() {
  if(Serial.available() == 0) return;
  
  String input = Serial.readStringUntil('\n');
  int value = input.toInt();
  
  if(value == 0 && input != "0") return;
  return value;
}
