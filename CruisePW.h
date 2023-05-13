#pragma once

class Cruise : public Mode
{
  private:
    float currentVoltage = 0;
    bool enabled = false;

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
      lcd.setCursor(1, 0);
      

      if(!enabled)
      {
        lcd.print("Currently disabled");
        setLEDs(false);
        return;
      } 

      lcd.print("Currently ENABLED ");
      setRelays(true);
      SetVoltage(currentVoltage);
      walkingLEDs();

      // Reset if clutch or brake are pressed
      if(CBPressed())
      {
        enabled = false;
        reset();
      }
    
      TM1638.displayText("--------");
    }

    void Trigger6()
    {
      enabled = !enabled;

      if(!enabled) { reset(); return; }

      // to enabled, setup to current throttle position
      setRelays(true);

      // Only set new voltage if throttle is pressed, otherwise disable cruise control
      if(ThrottlePressed()) currentVoltage = GetThrottle();
      else reset();
    }

    void Trigger7()
    {
      currentVoltage -= 0.1;

      // Disable cruise control
      if(currentVoltage < 0.6) reset();
    }

    void Trigger8()
    {
      currentVoltage += 0.1;

      if(currentVoltage > 5) currentVoltage = 5;
    }

    void reset()
    {
      //enabled = false;
      setRelays(false);
      SetVoltage(0);
    }

    bool CBPressed()
    {
      return ClutchPressed() || BrakePressed();
    }
};
