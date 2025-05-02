#include "PortHandler.h"

PortHandler::PortHandler(const int maxPorts) {
    this->maxPorts = maxPorts;               // Speichert die maximale Anzahl der Ports (keine Subtraktion!)
    this->ports = new int[maxPorts];         // Allokiert den Speicher für das Array
    for (int i = 0; i < maxPorts; ++i) {
        this->ports[i] = -1;                 // Initialisiert jeden Port auf -1
    }
    this->usage = 0;
}

PortHandler::~PortHandler() {
    delete[] this->ports;                    // Gibt den allokierten Speicher frei
}

void PortHandler::listPorts() {
    // Iteriert über alle Array-Elemente (von 0 bis maxPorts-1)
    for (int i = 0; i < this->maxPorts; ++i) {
        Serial.println("Port[" + String(i) + "] = " + String(this->ports[i]));
    }
}

void PortHandler::initPort(const int Port) {
    if (Port < 0) return;
    // Überprüft, ob noch ein freier Platz vorhanden ist
    if (usage < maxPorts) {
        this->ports[usage] = Port;
        ++usage;
    } else {
        Serial.println("Keine freien Ports verfügbar.");
    }
}

void PortHandler::delPort(const int Port) {
    // Sucht den Index des zu löschenden Ports
    int index = -1;
    for (int i = 0; i < usage; ++i) {
        if (ports[i] == Port) {
            index = i;
            break;
        }
    }
    // Wenn der Port nicht gefunden wird, wird eine Meldung ausgegeben
    if (index == -1) {
        Serial.println("Port nicht gefunden.");
        return;
    }
    // Verschiebt alle nachfolgenden Elemente nach links
    for (int i = index; i < usage - 1; ++i) {
        ports[i] = ports[i + 1];
    }
    // Setzt den letzten Port auf den Initialwert -1
    ports[usage - 1] = -1;
    --usage;
}
