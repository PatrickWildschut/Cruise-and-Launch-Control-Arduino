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
      setRelays(false);
      SetVoltage(idleVoltage);

      CurrentMode = 0;
      readIndex = 0;

      ShowBanner("        Read", true);
    }
    
    void Loop()
    {
      switch(readIndex)
      {
        case 0:
          readPedalsAndSpeed();
        break;
        case 1:
          readUtilities();
        break;
      }

      delay(20);
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

    void readPedalsAndSpeed()
    {
      short percentage = GetThrottlePercentage();

      // Display to I2C
      lcd.setCursor(0, 0);
      lcd.print("Throttle:     " + percentageText(percentage));
      lcd.setCursor(17, 0);
      lcd.print("%");

      lcd.setCursor(0, 1);
      lcd.print("Clutch:       " + String(ClutchPressed() ? "++++" : "    "));

      lcd.setCursor(0, 2);
      lcd.print("Brake:        " + String(BrakePressed() ? "++++" : "    "));

      lcd.setCursor(0, 3);
      lcd.print("Speed:        " + String(GetSpeed()));

      LEDsBasedOnPercentage(percentage);
      TM1638.displayText("    " + String(int(GetSpeed())));
    }

    String percentageText(short percentage)
    {
      if(percentage < 10)
      {
        return String(percentage) + "  ";
      }

      return String(percentage) + " ";
    }

    short GetThrottlePercentage()
    {
      float throttle = GetThrottle() - 0.6;
      int percentage = throttle / 4.0 * 100.0;

      if(percentage <= 0) return 0;

      if(percentage >= 100) return 100;

      return percentage;
    }

    void readUtilities()
    {
      lcd.setCursor(0, 0);
      lcd.print("G Force: " + String(GetGForce())  + "g");

      lcd.setCursor(0, 1);
      lcd.print("a:       " + String(GetAcceleration()) + " m/s2");

      lcd.setCursor(0, 3);
      lcd.print("Throttle:    " + String(GetThrottle()));
      lcd.setCursor(18, 3);
      lcd.print("V");

      LEDsBasedOnPercentage(GetThrottlePercentage());
    }
};
