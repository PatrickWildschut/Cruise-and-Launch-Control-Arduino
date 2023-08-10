#pragma once

class About : public Mode
{
  private:
    byte aboutIndex = 0;
    byte totalModes = 2;
  
  public:
    void Setup()
    {
      setRelays(false);
      SetVoltage(idleVoltage); 
    
      ShowBanner("       About", false);
      TM1638Banner("E  00  3");
    }
    
    void Loop()
    {
      AboutPage();
      delay(100);
    }

    void ButtonReceiver(short button)
    {
      switch (button)
      {
        case 8: case 16:
          Lock();
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
      lcd.print("Version: 0.9.4");

      walkingLEDs();
    }

    void Lock()
    {
      CurrentMode = 5;
      TM1638.reset();
      lcd.clear();

      Modes[CurrentMode]->Setup();

      // lock car
      LoggedIn = false;
      
    }
};
