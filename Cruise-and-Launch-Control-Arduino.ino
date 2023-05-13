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
  setRelays(true);
  
  lcd.init(); 
  lcd.backlight();
  MCP4725.begin(0x60);
  SetVoltage(0.5);

  pinMode(ThrottleIn, INPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);

  //wakeDemo();

  Modes[0] = static_cast<Mode *>(new Read());
  Modes[1] = static_cast<Mode *>(new Cruise());
  Modes[2] = static_cast<Mode *>(new Launch());
  Modes[3] = static_cast<Mode *>(new Test());
  Modes[4] = static_cast<Mode *>(new Login());

  setupMode(CurrentMode);
}

void setupMode(int newMode)
{
  lcd.clear();
  TM1638.reset();

  CurrentMode = newMode;

  Modes[CurrentMode]->Setup();
}

void loop() {
  handleButtons();

  Modes[CurrentMode]->Loop();
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

  if(buttons == 0) return;

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
