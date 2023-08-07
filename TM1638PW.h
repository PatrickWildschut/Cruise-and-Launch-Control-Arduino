#pragma once
#include <TM1638lite.h>

// strobe, clock, data
TM1638lite TM1638(6, 3, 5);

short index = 0;
bool inverse = true;

// check for clicked button on TM1638
bool ButtonClicked;

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

  TM1638.setLED(0, 0);
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
bool TM1638OnClick()
{
  if(ButtonClicked)
  {
    return false;
  }

  ButtonClicked = true;
  return true;
}

void TM1638Banner(String banner)
{
  TM1638.displayText(banner);
}

void enableLEDsAmount(short amount)
{
  for(int i = 0; i < amount; i++)
  {
    TM1638.setLED(i, 1);
  }

  for(int i = 7; i >= amount; i--)
  {
    TM1638.setLED(i, 0);
  }
}

void LEDsBasedOnPercentage(short percentage)
{
  short increment = 11;

  for(short i = 1; i < 10; i++)
  {
    if(increment * i >= percentage)
    {
      enableLEDsAmount(i - 1);
      break;
    }
  }
  
}
