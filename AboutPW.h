#pragma once

class About : public Mode
{
  private:
    byte aboutIndex = 0;
    byte totalModes = 2;
    float volt = 1;
  
  public:
    void Setup()
    {
      setRelays(false);
      SetVoltage(idleVoltage); 
    
      ShowBanner("       About", false);
      TM1638Banner("E. 00 .3");
    }
    
    void Loop()
    {
      switch(aboutIndex)
      {
        case 0:
          AboutPage();
        break;
        case 1:
          DACTest();
        break;
      }

      delay(100);
    }

    void ButtonReceiver(short button)
    {
      switch (button)
      {
        case 2:
          AboutBack();
          break;
        case 8: case 16:
          Lock();
          break;
        case 64:
          AboutNext();
          break;
      }
    }

    void AboutPage()
    {
      setRelays(false);

      lcd.setCursor(7, 0);
      lcd.print("Author:");

      lcd.setCursor(2, 1);
      lcd.print("Patrick Wildschut");

      lcd.setCursor(3, 2);
      lcd.print("Copyright 2023");

      lcd.setCursor(3, 3);
      lcd.print("Version: 0.9.1");

      walkingLEDs();
    }

    void DACTest()
    {
      setRelays(true);

      lcd.setCursor(0, 0);
      lcd.print("Current volt: " + String(volt));
      SetVoltage(volt);
    }

    void Lock()
    {
      CurrentMode = 4;
      TM1638.reset();
      lcd.clear();

      Modes[CurrentMode]->Setup();

      // lock car
      LoggedIn = false;
      
    }
  
    void AboutBack()
    {
      lcd.clear();
      aboutIndex -= 1;

      if(aboutIndex < 0) aboutIndex = totalModes - 1;
    }
  
    void AboutNext()
    {
      lcd.clear();
      aboutIndex += 1;

      if(aboutIndex > totalModes - 1) aboutIndex = 0;
    }
};
