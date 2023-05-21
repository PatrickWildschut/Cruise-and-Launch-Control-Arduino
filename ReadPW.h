#pragma once

class Read : public Mode
{
  
  public:
    void Setup()
    {
      SetVoltage(idleVoltage);
      setRelays(false);

      CurrentMode = 0;
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

      lcd.setCursor(0, 3);
      lcd.print("Speed:        " + String(GetSpeed()));
    
      // TEST
      TM1638.displayText("Reading");
    }

    void Trigger5(){}

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
