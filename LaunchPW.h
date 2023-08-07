#pragma once

class Launch : public Mode
{
  private:
    bool isPressedBefore = false;
    bool started = false;

  public:
    void Setup()
    {
      setRelays(false);
      SetVoltage(idleVoltage);

      reset();
      CurrentMode = 2;
    
      ShowBanner("   Launch Control", true);
    }
    
    void Loop()
    {
      if(ClutchPressed() && !started)
      {
        // Clutch is pressed, rev until release, then go into 'started'
        lcd.setCursor(1, 0);
        lcd.print("Revving to 4k RPM");

        lcd.setCursor(1, 1);
        lcd.print("Release clutch to");

        lcd.setCursor(7, 2);
        lcd.print("LAUNCH");

        flashLEDs();
        setRelays(true);
        SetVoltage(1.3);
        isPressedBefore = true;  
      } else
      {
        if(isPressedBefore)
        {
          // LAUNCH
          LCDreset(0);
          LCDreset(2);
          setRelays(false);
          SetVoltage(idleVoltage);

          lcd.setCursor(0, 1);
          lcd.print("    LAUNCHED!!!   ");

          setLEDs(true);
          started = true;
        } else
        {
          // BEFORE CLUTCH DEPRESSED

          lcd.setCursor(3, 0);
          lcd.print("Depress clutch");

          lcd.setCursor(6, 1);
          lcd.print("to start");

          setRelays(false);
          walkingLEDs();
        }
      }

      TM1638.displayText("--------");

      delay(100);
    }

    void ButtonReceiver(short button)
    {
      reset();
    }

    void reset()
    {
      started = false;
      isPressedBefore = false;
    }
};
