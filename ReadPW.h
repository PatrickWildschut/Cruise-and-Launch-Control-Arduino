#pragma once

class Read : public Mode
{
  
  public:
    void Setup()
    {
      CurrentMode = 0;
    
      SetVoltage(0);
    
      setBanner("Current mode: Read");
      setRelays(false);
    }
    
    void Loop()
    {
    
      // Display to I2C
      lcd.setCursor(0, 0);
      lcd.print("Throttle:     " + String(GetThrottle()) + String(ThrottlePressed() ? " +" : "  "));

      lcd.setCursor(0, 1);
      lcd.print("Clutch:       " + String(ClutchPressed() ? "++++" : "    "));

      lcd.setCursor(0, 2);
      lcd.print("Brake:        " + String(BrakePressed() ? "++++" : "    "));
    
      // TEST
      TM1638.displayText("Reading");
    }

    void Trigger6()
    {
      
    }

    void Trigger7()
    {
      
    }
  
    void Trigger8()
    {
      
    }
};
