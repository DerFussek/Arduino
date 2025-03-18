#include "Arduino.h"
class InputHandler {
  int _SWup, _SWdown;
  public:
    InputHandler(int SWup, int SWdown) {
      this->_SWup = SWup;
      this->_SWdown = SWdown;
    }

  void setup() {
    pinMode(this->_SWup, INPUT_PULLUP);
    pinMode(this->_SWdown, INPUT_PULLUP);
  }

  bool SWup() {
    if(!digitalRead(this->_SWup)) return true;
    else return false;
  }

  bool SWdwon() {
    if(!digitalRead(this->_SWdown)) return true;
    else return false;
  }
};