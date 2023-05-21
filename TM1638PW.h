#pragma once
#include <TM1638lite.h>

// strobe, clock, data
TM1638lite TM1638(6, 3, 5);

short index = 0;
bool inverse = true;

// buttons on TM1638
bool Buttons[8];

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
  index = 0;
  for(int i = 0; i < 8; i++)
  {
    TM1638.setLED(i, inverse);
  }

  inverse = !inverse;
}

void setLEDs(bool enabled)
{
  index = 0;

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

void TM1638arrowDisplay(bool right)
{
  if(right)
  {
    TM1638.displayText("-------3");
  } else{
    TM1638.displayText("E-------");
  }
}

// This checks if the button was already pressed, if so false, otherwise true. Used for button handling
bool checkSetButtonClicked(short index)
{
  if(Buttons[index])
  {
    return false;
  }

  Buttons[index] = true;
  return true;
}
