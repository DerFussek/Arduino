#pragma once
#include "Arduino.h"

class PortHandler {
    private:
      int *ports;
      int maxPorts; // Maximale Anzahl der Ports
      int usage;    // Anzahl der aktuell genutzten Ports
    public:
      PortHandler(const int maxPorts);
      ~PortHandler(); // Destruktor zum Freigeben des Speichers
      
      void listPorts();
      void initPort(const int Port);
      void delPort(const int Port);
};
