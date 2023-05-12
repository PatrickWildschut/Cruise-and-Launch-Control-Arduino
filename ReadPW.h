#pragma once

class Read : public Mode
{
  private:
    float valueT = 0;
    float valueC = 0;
    float valueB = 0;
  
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
      // Read pins
      valueT = analogRead(ThrottleIn) * 5.0 / 1023.0;

      valueC = analogRead(ClutchIn) * 5.0 / 1023.0;

      valueB = analogRead(BrakeIn) * 5.0 / 1023.0;
    
      // Display to I2C
      lcd.setCursor(0, 0);
      lcd.print("Throttle:     " + String(valueT) + (valueT > 0.7 ? " +" : "  "));

      lcd.setCursor(0, 1);
      lcd.print("Clutch:       " + String(valueC));

      lcd.setCursor(0, 2);
      lcd.print("Brake:        " + String(valueB));
    
      // TEST
      TM1638.displayText("Reading");
    }

    void Trigger7()
    {
      
    }
  
    void Trigger8()
    {
      
    }
};
