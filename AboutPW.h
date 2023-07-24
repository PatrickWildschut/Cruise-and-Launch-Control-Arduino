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

    void AboutPage()
    {
      lcd.setCursor(7, 0);
      lcd.print("Author:");

      lcd.setCursor(2, 1);
      lcd.print("Patrick Wildschut");

      lcd.setCursor(3, 2);
      lcd.print("Copyright 2023");

      lcd.setCursor(3, 3);
      lcd.print("Version: 0.8.4");

      walkingLEDs();
    }

    void DACTest()
    {
      setRelays(true);

      lcd.setCursor(0, 0);
      lcd.print("Current volt: " + String(volt));
      SetVoltage(volt);
    }

    void Trigger5()
    {
      CurrentMode = 4;

      Modes[CurrentMode]->Setup();

      // lock car
      LoggedIn = false;
      
    }

    void Trigger6()
    {
      volt += 0.1;
    }
  
    void Trigger7()
    {
      lcd.clear();
      aboutIndex -= 1;

      if(aboutIndex < 0) aboutIndex = totalModes - 1;
    }
  
    void Trigger8()
    {
      lcd.clear();
      aboutIndex += 1;

      if(aboutIndex > totalModes - 1) aboutIndex = 0;
    }
};
