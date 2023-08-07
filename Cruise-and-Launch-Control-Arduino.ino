/*

Todo:
Launch control mode will be replaced with miscellaneous mode

miscellaneous mode:
this mode has will have a speed limiter, launch control, the return of trip master

Settings:
Layouts -> Horizontal (old), vertical (new)

All settings will be saved to the arduino EEPROM

*/

// used to save settings.
#include <EEPROM.h>

#include "LCDPW.h"
#include "DACPW.h"
#include "TM1638PW.h"

#include "Mode.h"
#include "GlobalPW.h"

#include "LoginPW.h"
#include "CruisePW.h"
#include "LaunchPW.h"
#include "ReadPW.h"
#include "SettingsPW.h"
#include "AboutPW.h"

void setup() {
  // put your setup code here, to run once:
  lcd.init(); 
  lcd.backlight();
  MCP4725.begin(0x60);
  //TWBR = 12;

  pinMode(ThrottleIn, INPUT);
  pinMode(SpeedIn, INPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);

  Modes[0] = static_cast<Mode *>(new Read());
  Modes[1] = static_cast<Mode *>(new Cruise());
  Modes[2] = static_cast<Mode *>(new Launch());
  Modes[3] = static_cast<Mode *>(new Settings());
  Modes[4] = static_cast<Mode *>(new About());
  Modes[5] = static_cast<Mode *>(new Login());

  LoadLayouts();

  //Modes[CurrentMode]->Setup();
  setupMode(CurrentMode);
}

void setupMode(int index)
{
  lcd.clear();
  TM1638.reset();

  if(CurrentMode < 0) CurrentMode = TotalModes;
  if(CurrentMode > TotalModes) CurrentMode = 0;

  if(LoggedIn)
  {
    Modes[CurrentMode]->Setup();
  } else{
    Modes[5]->Setup();
  }
}

void loop() {
  handleButtons();

  if(LoggedIn)
    Modes[CurrentMode]->Loop();
  else
    Modes[5]->Loop();   
  
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
    // logged in check
    if(!LoggedIn)
    {
      Modes[5]->ButtonReceiver(buttons);
      return;
    }

    switch(buttons)
    {
      // Previous mode 1
      case 1:
        CurrentMode -= 1;
        setupMode(CurrentMode);
        break;
      
      // Next mode button 8
      case 128:
        CurrentMode += 1;
        setupMode(CurrentMode);
        break;

      // all mode dependend buttons
      default:
        Modes[CurrentMode]->ButtonReceiver(buttons);
        break;
    }
  }
}

