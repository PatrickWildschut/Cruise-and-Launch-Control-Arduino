#pragma once

class Cruise : public Mode
{
  private:
    float currentVoltage = 0;
    int desiredSpeed = -1;
    bool enabled = false;
    bool right = false;
    byte speedChangeFeedbackIndex = 0;

  public:
    void Setup()
    {
      SetVoltage(idleVoltage);
      setRelays(false);

      CurrentMode = 1;
      enabled = false;
      reset();
      
      setBanner("   Cruise Control  ");
    }
    
    void Loop()
    {
      lcd.setCursor(1, 0);
      

      if(!enabled)
      {
        lcd.print("Currently disabled");
        TM1638.displayText("--------");
        setLEDs(false);
        return;
      } 

      cruiseEnabled();

      // speed changed, show arrow on TM1638 and feedback on LCD for `speedChangeFeedbackIndex` amount of time
      if(speedChangeFeedbackIndex > 0)
      {
        speedChangeFeedback();
      } else
      {
        // speed not changed, show current voltage on TM1638.
        TM1638.displayText("up  " + String(currentVoltage));
      }

      // Reset if any pedal is pressed
      if(PedalPressed())
      {
        enabled = false;
        reset();
      }
    }

    void Trigger5()
    {
      // SWITCH ON OFF
      // RESET TO OLD DESIRED SPEED

      enabled = !enabled;

      if(!enabled) { reset(); return; }

      setCurrentSpeed();
    }

    void Trigger6()
    {
      // SET TO CURRENT SPEED
      if(!enabled)
      {
        enabled = true;

        if(desiredSpeed <= 30)
          setCurrentSpeed();
      } else{
        // already enabled, increase speed by 5.
        desiredSpeed += 5;
      }
        

      
    }

    void Trigger7()
    {
      desiredSpeed -= 1;

      speedChangeFeedbackIndex = 5;
      right = false;

      // Disable cruise control if desired speed too low
      if(desiredSpeed < 30) reset();
    }

    void Trigger8()
    {
      desiredSpeed += 1;

      speedChangeFeedbackIndex = 5;
      right = true;
    }

    void reset()
    {
      lcd.clear();
      setRelays(false);
      currentVoltage = idleVoltage;
      SetVoltage(idleVoltage);
    }

    void cruiseEnabled()
    {
      lcdCruising();

      currentVoltage = calculateOptimalThrottlePosition();

      SetVoltage(currentVoltage);

      setRelays(true);
      walkingLEDs();
    }

    bool PedalPressed()
    {
      return ClutchPressed() || BrakePressed() || ThrottlePressed();
    }

    void speedChangeFeedback()
    {
      TM1638arrowDisplay(right);
      speedChangeFeedbackIndex -= 1;
    }

    float throttleBasedOnDesiredSpeed()
    {
      // these static values are based on testing results

      int difference = desiredSpeed - 50;

      return 1.15 + (difference * 0.01);
    }

    float calculateOptimalThrottlePosition()
    {
      // Compare speeds
      int speedDifference = desiredSpeed - GetSpeed(); 

      // Optimal throttle position formula by me :)
      float ots = throttleBasedOnDesiredSpeed() + pow(speedDifference, 2) * 0.01;

      // lock to max and min voltage
      if(ots > 3)
      {
        ots = 3;
      } else if(ots < 0.6)
      {
        ots = 0.6;
      }

      return ots;
    }

    void lcdCruising()
    {
      lcd.setCursor(0, 0);
      lcd.print("  Currently ENABLED ");

      lcd.setCursor(0, 1);
      lcd.print("Desired speed: " + String(desiredSpeed));

      lcd.setCursor(0, 2);
      lcd.print("Current voltage:" + String(currentVoltage));
    }

    void setCurrentSpeed()
    {
      // set to current speed if desired speed wasnt assigned yet
      float currentSpeed = GetSpeed();

      if(currentSpeed >= 30)
      {
        desiredSpeed = currentSpeed;
      } else{
        enabled = false;
        reset();
      }
    }
};
