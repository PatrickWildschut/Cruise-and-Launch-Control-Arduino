#pragma once

class Cruise : public Mode
{
  public:
    void Setup()
    {
      CurrentMode = 1;
      SetVoltage(0);
      
      setBanner("Current mode: Cruise");
    
      setRelays(false);
    }
    
    void Loop()
    {
      lcd.setCursor(0, 0);
      lcd.print("To do...");
    
      TM1638.displayText("--------");
    }

    void Trigger7()
    {
      
    }

    void Trigger8()
    {
      
    }
};
