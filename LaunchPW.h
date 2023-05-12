#pragma once

class Launch : public Mode
{
  public:
    void Setup()
    {
      CurrentMode = 2;
      SetVoltage(0);
    
      setBanner("Current mode: Launch");
    
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
