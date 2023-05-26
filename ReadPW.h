#pragma once

class Read : public Mode
{
  private:
    // current mode within read mode
    short readIndex = 0;
    const short totalModes = 2;
  
  public:
    void Setup()
    {
      SetVoltage(idleVoltage);
      setRelays(false);

      CurrentMode = 0;
      readIndex = 0;
    }
    
    void Loop()
    {
      switch(readIndex)
      {
        case 0:
          readPedalsSpeed();
        break;
        case 1:
          readUtilities();
        break;
      }
    }

    void Trigger5(){}

    void Trigger6(){}

    void Trigger7()
    {
      lcd.clear();
      readIndex -= 1;

      if(readIndex < 0) readIndex = totalModes - 1;
    }
  
    void Trigger8()
    {
      lcd.clear();
      readIndex += 1;

      if(readIndex > totalModes - 1) readIndex = 0;
    }

    void readPedalsSpeed()
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

      TM1638.displayText("Reading");
    }

    void readUtilities()
    {
      lcd.setCursor(0, 0);
      lcd.print("G Force: " + String(GetGForce())  + "g");

      lcd.setCursor(0, 1);
      lcd.print("Aceleration:" + String(GetAcceleration()) + "m/s2");
    }
};
