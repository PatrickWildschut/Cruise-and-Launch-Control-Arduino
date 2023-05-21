#include "Mode.h"
#include "GlobalPW.h"

#include "LCDPW.h"
#include "DACPW.h"
#include "TM1638PW.h"


#include "LoginPW.h"
#include "CruisePW.h"
#include "LaunchPW.h"
#include "ReadPW.h"
#include "TestPW.h"

void setup() {
  // put your setup code here, to run once:
  lcd.init(); 
  lcd.backlight();
  MCP4725.begin(0x60);

  pinMode(ThrottleIn, INPUT);
  pinMode(SpeedIn, INPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);

  SetVoltage(0);
  SetVoltage(idleVoltage);
  setRelays(true);

  wakeDemo();

  Modes[0] = static_cast<Mode *>(new Read());
  Modes[1] = static_cast<Mode *>(new Cruise());
  Modes[2] = static_cast<Mode *>(new Launch());
  Modes[3] = static_cast<Mode *>(new Test());
  Modes[4] = static_cast<Mode *>(new Login());

  //Modes[CurrentMode]->Setup();
  setupMode(CurrentMode);
}

void setupMode(int index)
{
  lcd.clear();
  TM1638.reset();

  if(LoggedIn)
  {
    CurrentMode = index;

    Modes[CurrentMode]->Setup();
  } else{
    Modes[4]->Setup();
  }
}

void loop() {
  handleButtons();

  if(LoggedIn)
    Modes[CurrentMode]->Loop();
  else
    Modes[4]->Loop();
    
  delay(100);
}

void wakeDemo()
{
  LCDDemo();
  for(int i = 0; i < 7; i++)
  {
    demoLEDs();
  }

  lcd.clear();
}

void handleButtons()
{
  byte buttons = TM1638.readButtons();

  if(buttons == 0) 
  {
    // reset all buttons to 'not clicked'
    for(int i = 0; i < 8; i++)
    {
      Buttons[i] = false;
    }

    return;
  }

  switch(buttons)
  {
    // button1 pressed
    case 1:
      checkSetButtonClicked(0) ? setupMode(0) : void();
      break;
    // button2 pressed
    case 2:
      checkSetButtonClicked(1) ? setupMode(1) : void();
      break;
    // button3 pressed
    case 4:
      checkSetButtonClicked(2) ? setupMode(2) : void();
      break;
    // button4 pressed
    case 8:
      checkSetButtonClicked(3) ? setupMode(3) : void();
      break;
    // button5 pressed
    case 16:
      checkSetButtonClicked(4) ? Modes[CurrentMode]->Trigger5() : void();
      break;
    // button6 pressed
    case 32:
      checkSetButtonClicked(5) ? Modes[CurrentMode]->Trigger6() : void();
      break;
    // button7 pressed
    case 64:
      checkSetButtonClicked(6) ? Modes[CurrentMode]->Trigger7() : void();
      break;
    // button8 pressed
    case 128:
      checkSetButtonClicked(7) ? Modes[CurrentMode]->Trigger8() : void();
      break;
  }
}
