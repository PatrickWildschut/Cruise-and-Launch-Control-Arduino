#include "LCDPW.h"
#include "DACPW.h"
#include "TM1638PW.h"

#include "Mode.h"
#include "GlobalPW.h"

#include "LoginPW.h"
#include "CruisePW.h"
#include "LaunchPW.h"
#include "ReadPW.h"
#include "AboutPW.h"

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

  Modes[0] = static_cast<Mode *>(new Read());
  Modes[1] = static_cast<Mode *>(new Cruise());
  Modes[2] = static_cast<Mode *>(new Launch());
  Modes[3] = static_cast<Mode *>(new About());
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
    
  
} // every Loop function within Mode must contain a delay()

void handleButtons()
{
  byte buttons = TM1638.readButtons();

  if(buttons == 0) 
  {
    ButtonClicked = false;

    return;
  }

  // on click check
  // check if it wasn't already pressed
  if(TM1638OnClick())
  {
    switch(buttons)
    {
      // button1 pressed
      case 1:
        setupMode(0);
        break;
      // button2 pressed
      case 2:
        setupMode(1);
        break;
      // button3 pressed
      case 4:
        setupMode(2);
        break;
      // button4 pressed
      case 8:
        setupMode(3);
        break;
      // button5 pressed
      case 16:
        Modes[CurrentMode]->Trigger5();
        break;
      // button6 pressed
      case 32:
        Modes[CurrentMode]->Trigger6();
        break;
      // button7 pressed
      case 64:
        Modes[CurrentMode]->Trigger7();
        break;
      // button8 pressed
      case 128:
        Modes[CurrentMode]->Trigger8();
        break;
    }
  }
  
}
