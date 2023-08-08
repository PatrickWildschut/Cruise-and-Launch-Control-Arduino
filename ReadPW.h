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

      readIndex = 0;

      ShowBanner("        Read", true);

      TM1638Banner("E.    .3");
    }
    
    void Loop()
    {
      switch(readIndex)
      {
        case 0:
          Default();
        break;
        case 1:
          readUtilities();
        break;
      }

      LEDsBasedOnPercentage(GetThrottlePercentage());

      delay(20);
    }

    void ButtonReceiver(short button)
    {
      switch(button)
      {
        case 2:
          ReadBack();
          break;
        case 64:
          ReadNext();
          break;
      }
    }

    void ReadBack()
    {
      lcd.clear();
      readIndex -= 1;

      if(readIndex < 0) readIndex = totalModes - 1;
    }
  
    void ReadNext()
    {
      lcd.clear();
      readIndex += 1;

      if(readIndex > totalModes - 1) readIndex = 0;
    }

    void Default()
    {
      ReadModeLayout == 0 ? Horizontal() : Vertical();
    }

    void Horizontal()
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
      //TM1638.displayText("    " + String(int(GetSpeed())));
    }

    void Vertical()
    {
      short percentage = GetThrottlePercentage();

      // Display to I2C
      lcd.setCursor(1, 0);
      lcd.print("Throttle    Speed");

      lcd.setCursor(4, 1);
      lcd.print(percentageText(percentage));

      lcd.setCursor(7, 1);
      lcd.print("%");

      lcd.setCursor(13, 1);
      lcd.print(String(GetSpeed()));

      lcd.setCursor(2, 2);
      lcd.print("Clutch     Brake");

      lcd.setCursor(2, 3);
      lcd.print(String(ClutchPressed() ? "++++++" : "      "));

      lcd.setCursor(13, 3);
      lcd.print(String(BrakePressed() ? "+++++" : "     "));

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
      lcd.print("G Force: " + String(GetGForce()));
      lcd.setCursor(16, 0);
      lcd.print("G");

      lcd.setCursor(0, 1);
      lcd.print("a:       " + String(GetAcceleration()));
      lcd.setCursor(15, 1);
      lcd.print("m/s2");

      lcd.setCursor(0, 3);
      lcd.print("Throttle:    " + String(GetThrottle()));
      lcd.setCursor(18, 3);
      lcd.print("V");
    }
};
