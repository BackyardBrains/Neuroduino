/*
  Neuroduino.h
*/
#ifndef Neuroduino_h
#define Neuroduino_h

#include "Arduino.h"

class Neuroduino
{
  public:
    Neuroduino(void);
    void writeLEDs(void);
    void writeLEDs(byte outByte);
    void writeLED(int led, bool state);
  private:
    byte _shiftRegState = 0;
};

#endif