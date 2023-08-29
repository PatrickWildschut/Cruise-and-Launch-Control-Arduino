#pragma once

class Cruise : public Mode
{
  private:
    const byte minSpeed = 30;
    const float baseVoltage50 = 1.10;

    float currentVoltage = 0;
    float oldSetVoltage = 0;
    short averageSpeedSize = 10;
    float averageSpeed[10];
    float currentSpeed = 0;
    short desiredSpeed = -1;
    bool enabled = false;
    bool increaseDesiredSpeed = false;
    byte speedChangeFeedbackIndex = 0;

  public:
    void Setup()
    {

      setRelays(false);
      SetVoltage(idleVoltage);

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
        
        TM1638Banner("Eno  <>3");
        setRelays(false);
        setLEDs(false);
        return;
      }  

      // cruise control is enabled

      // choose layout
      CruiseModeLayout == 0 ? Horizontal() : Vertical();

      TM1638Banner("E0  <>>3");
      walkingLEDs();

      controlSpeed();
      checkDesiredSpeedChanged();      

      checkPedalsPressed();

      delay(100);
    }

    void ButtonReceiver(short button)
    {
      if(enabled)
      {
        switch(button)
        {
          case 2:
            enabled = false;
            reset();
            break;
          case 16:
            ChangeDesiredSpeed(-1, false);
            break;
          case 32:
            ChangeDesiredSpeed(1, false);
            break;
          case 64:
            ChangeDesiredSpeed(5, true);
            break;
        }
      } else
      {
        // not enabled, buttons:
        switch(button)
        {
          case 2:
            OnNewSpeed();
            break;
          case 4:
            OnOldSpeed();
            break;
          case 32:
            ChangeDesiredSpeed(-5, true);
            break;
          case 64:
            ChangeDesiredSpeed(5, true);
            break;
        }
      }
      
    }

    void OnNewSpeed()
    {
      // SWITCH ON
      // GET NEW SPEED BASED ON CURRENT SPEED
      if(!enabled)
      {
        enabled = true;

        setCurrentSpeed();
      }
    }

    void OnOldSpeed()
    {
      // KEEP OLD DESIRED SPEED, UNLESS DESIRED SPEED IS TOO LOW
      if(!enabled)
      {
        enabled = true;

        if(desiredSpeed <= minSpeed)
          setCurrentSpeed();

        return;
      }
    }

    void ChangeDesiredSpeed(int amount, bool snap)
    {
      bool positive = amount > 0 ? true : false;

      // show on TM1638 display little animation
      speedChangeFeedbackIndex = 5;
      increaseDesiredSpeed = positive;

      if(snap)
      {
        int remainder = desiredSpeed % amount;

        desiredSpeed += amount - remainder;
      } else
      {
        desiredSpeed += amount;
      }

      if(desiredSpeed < minSpeed) desiredSpeed = minSpeed;

    }

    void controlSpeed()
    {
      currentVoltage = calculateOptimalThrottlePosition();

      float difference = currentVoltage - oldSetVoltage;

      // only update DAC if there is a difference to avoid EAC fail
      if(difference != 0)
      {
        SetVoltage(currentVoltage);

        oldSetVoltage = currentVoltage;
      }
    }

    void checkDesiredSpeedChanged()
    {
      // if speed changed, show arrow on TM1638 and feedback on LCD for `speedChangeFeedbackIndex` amount of time
      if(speedChangeFeedbackIndex > 0)
      {
        speedChangeFeedback();
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
      bool enabled = !ThrottlePressed(currentVoltage - 0.3);
      
      setRelays(enabled);

      lcd.setCursor(0, 0);
      if(enabled)
      {
        lcd.print(" Currently ENABLED ");
      } else{
        lcd.print(" Release Throttle..");
      }
      
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

    void Horizontal()
    {
      lcd.setCursor(0, 1);
      lcd.print("Desired speed: " + String(desiredSpeed));

      lcd.setCursor(0, 2);
      lcd.print("Current speed:" + String(currentSpeed));
    }

    void Vertical()
    {
      lcd.setCursor(7, 1);
      lcd.print("Speed");

      lcd.setCursor(0, 2);
      lcd.print(" Current   Desired  ");

      lcd.setCursor(2, 3);
      lcd.print(String(currentSpeed));

      lcd.setCursor(13, 3);
      lcd.print(String(desiredSpeed));
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
