#pragma once

class Cruise : public Mode
{
  private:
    float currentVoltage = 0;
    float currentSpeed = 0;
    int desiredSpeed = -1;
    bool enabled = false;
    bool increaseDesiredSpeed = false;
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
        setRelays(false);
        setLEDs(false);
        return;
      } 

      controlSpeed();

      checkDesiredSpeedChanged();      

      checkPedalsPressed();
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

        return;
      }

      // already enabled, increase speed by 5, also snap to closest 5 factor.
      int remainder = desiredSpeed % 5;

      if(remainder == 0) 
      {
        desiredSpeed += 5;
      } else{
        desiredSpeed += 5 - remainder;
      }
    }

    void Trigger7()
    {
      desiredSpeed -= 1;

      speedChangeFeedbackIndex = 5;
      increaseDesiredSpeed = false;

      // Disable cruise control if desired speed too low
      if(desiredSpeed < 30) reset();
    }

    void Trigger8()
    {
      desiredSpeed += 1;

      speedChangeFeedbackIndex = 5;
      increaseDesiredSpeed = true;
    }

    void controlSpeed()
    {
      lcdCruising();

      currentVoltage = calculateOptimalThrottlePosition();

      SetVoltage(currentVoltage);
      walkingLEDs();
    }

    void checkDesiredSpeedChanged()
    {
      // if speed changed, show arrow on TM1638 and feedback on LCD for `speedChangeFeedbackIndex` amount of time
      if(speedChangeFeedbackIndex > 0)
      {
        speedChangeFeedback();
      } else
      {
        // speed not changed, show current voltage on TM1638.
        TM1638.displayText("Up  " + String(currentVoltage));
      }
    }

    void checkPedalsPressed()
    {
      // clutch or brake are pressed, if so, reset
      if(CBPressed())
      {
        enabled = false;
        reset();
        return;
      }

      // Check if throttle is pressed in more than the voltage we are providing
      // if so, switch to physical pedal, otherwise, just keep cruise control enabled :)
      setRelays(!ThrottlePressed(currentVoltage));
    }

    void reset()
    {
      lcd.clear();
      setBanner("   Cruise Control  ");
      setRelays(false);
      currentVoltage = idleVoltage;
      SetVoltage(idleVoltage);
    }

    bool CBPressed()
    {
      return ClutchPressed() || BrakePressed();
    }

    void speedChangeFeedback()
    {
      TM1638arrowDisplay(increaseDesiredSpeed);
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
      float ots = 0;

      // Compare speeds
      currentSpeed = GetSpeed();
      float speedDifference = desiredSpeed - currentSpeed; 

      if(speedDifference < 0)
      {
        // subtracting because power 2 with negative numbers will get positive :)
        ots = throttleBasedOnDesiredSpeed() - pow(speedDifference, 2) * 0.01;
      } else{
        ots = throttleBasedOnDesiredSpeed() + pow(speedDifference, 2) * 0.01;
      }

      // Optimal throttle position formula by me :)
      

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
      lcd.print("Current speed:" + String(currentSpeed));
    }

    void setCurrentSpeed()
    {
      // set to current speed if desired speed wasnt assigned yet
      float speed = GetSpeed();

      if(currentSpeed >= 30)
      {
        desiredSpeed = speed;
      } else{
        enabled = false;
        reset();
      }
    }
};
