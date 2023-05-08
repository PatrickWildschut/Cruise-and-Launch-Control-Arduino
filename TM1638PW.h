#pragma once
#include <TM1638lite.h>

TM1638lite TM1638(6, 3, 5);

short index = 0;
bool inverse = true;

void walkingLEDs()
{
  TM1638.setLED(index, inverse);

  index++;
  
  if(index == 8)
  {
    index = 0;
    inverse = !inverse;
  }
}
