#pragma once

class Cruise : public Mode
{
  private:
    const int minSpeed = 30;
    const float baseVoltage50 = 1.17;

    float currentVoltage = 0;
    float oldSetVoltage = 0;
    short averageSpeedSize = 10;
    float averageSpeed[10];
    float currentSpeed = 0;
    int desiredSpeed = -1;
    bool enabled = false;
    bool increaseDesiredSpeed = false;
    byte speedChangeFeedbackIndex = 0;

  public:
    void Setup()
    {
      setRelays(false);
      SetVoltage(idleVoltage);

      CurrentMode = 1;
      enabled = false;
      reset();
      
      ShowBanner("   Cruise Control", true);
    }
    
    void Loop()
    {
      lcd.setCursor(1, 0);
      
      updateCurrentSpeed();

      if(!enabled)
      {
        lcd.print("Currently disabled");

        if(desiredSpeed != -1)
        {
          lcd.setCursor(2, 2);
          lcd.print("Saved speed:  " + String(desiredSpeed));
        }
        
        TM1638.displayText("--------");
        setRelays(false);
        setLEDs(false);
        return;
      } 

      controlSpeed();

      checkDesiredSpeedChanged();      

      checkPedalsPressed();

      delay(100);
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

        if(desiredSpeed <= minSpeed)
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
      if(desiredSpeed < minSpeed) reset();
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

      float difference = currentVoltage - oldSetVoltage;
      difference = difference > 0 ? difference : -difference;

      // only update DAC if there is a big enough difference to avoid EAC fail
      if(difference > 0.02)
      {
        SetVoltage(currentVoltage);

        oldSetVoltage = currentVoltage;
      }
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
      bool enabled = !ThrottlePressed(currentVoltage + 0.4);
      
      setRelays(enabled);

      // if(enabled)
      //   lcd.noBacklight();
      // else
      //   lcd.backlight();
      
    }

    void reset()
    {
      lcd.clear();
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

      return baseVoltage50 + (difference * 0.01);
    }

    float throttleBasedOnCurrentSpeed()
    {
      // these static values are based on testing results

      float difference = currentSpeed - 50.0;

      return baseVoltage50 + (difference * 0.01);
    }

    float calculateOptimalThrottlePosition()
    {
      float ots = 0;

      // Compare speeds
      float speedDifference = desiredSpeed - currentSpeed;

      if(speedDifference < 0)
      {
        // subtracting because power 2 with negative numbers will get positive :)
        ots = throttleBasedOnDesiredSpeed() - pow(speedDifference, 2) * 0.01;
      } else{
        ots = throttleBasedOnDesiredSpeed() + pow(speedDifference, 2) * 0.01;
      }

      // Optimal throttle position formula by me :)
      float tCS = throttleBasedOnCurrentSpeed();

      // lock to max and min voltage
      if(ots > tCS + 0.5)
      {
        ots = tCS + 0.5;
      } else if(ots < idleVoltage)
      {
        ots = idleVoltage;
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

      lcd.setCursor(0, 3);
      lcd.print("DEBUG:        " + String(oldSetVoltage) + "v");
    }

    void setCurrentSpeed()
    {
      // set to current speed if desired speed wasnt assigned yet
      float speed = GetSpeed();

      if(speed >= minSpeed)
      {
        desiredSpeed = speed;
      } else{
        enabled = false;
        reset();
      }
    }

    void updateCurrentSpeed()
    {
      // move all 1 to i - 1
      for(int i = averageSpeedSize - 1; i > 0; i--)
      {
        averageSpeed[i - 1] = averageSpeed[i];
      }
      averageSpeed[averageSpeedSize - 1] = GetSpeed();

      // count total
      float speed = 0;
      for(int i = 0; i < averageSpeedSize; i++)
      {
        speed += averageSpeed[i];
      }

      currentSpeed = speed / float(averageSpeedSize);
    }
};
