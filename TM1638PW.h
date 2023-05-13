#pragma once
#include <TM1638lite.h>

// strobe, clock, data
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

void flashLEDs()
{
  for(int i = 0; i < 8; i++)
  {
    TM1638.setLED(i, inverse);
  }

  inverse = !inverse;
}

void setLEDs(bool enabled)
{
  for(int i = 0; i < 8; i++)
  {
    TM1638.setLED(i, enabled);
  }
}

// helping functions
void demoLEDs()
{
  // to left
  for(int i = 0; i < 8; i++)
  {
    TM1638.setLED(i, 1);

    if(i > 0)
    {
      TM1638.setLED(i - 1, 0);
    }
    
    delay(50);
  }

  // to right

  for(int i = 7; i >= 0; i--)
  {
    TM1638.setLED(i, 1);

    if(i < 7)
    {
      TM1638.setLED(i + 1, 0);
    }
    
    delay(50);
  }
}
