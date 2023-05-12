#pragma once

class Read : public Mode
{
  private:
    float value3 = 0;
    float value6 = 0;
  
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
      value3 = analogRead(ThrottleIn1);
      value3 *= 5.0 / 1023.0;
      
      value6 = analogRead(ThrottleIn2);
      value6 *= 5.0 / 1023.0;
    
      // Display to I2C
      lcd.setCursor(0, 0);
      lcd.print("3: " + String(value3) + " volt");
    
      lcd.setCursor(0, 1);
      lcd.print("6: " + String(value6) + " volt");
    
      // TEST
      TM1638.displayText(String(value3) + String(value6));
    }

    void Trigger7()
    {
      
    }
  
    void Trigger8()
    {
      
    }
};
