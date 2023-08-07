#pragma once

class Misc : public Mode
{
  private:
    bool inMainMenu = true;
    byte cursorY = 1;
    byte currentSubMenu = 0;

    // Launch control
    bool isPressedBefore = false;
    bool started = false;

    // Trip master
    float totalDistance = 0;
    float currentDistance = 0;

  public:
    void Setup()
    {
      setRelays(false);
      SetVoltage(idleVoltage);

      reset();
    
      ShowBanner("   Miscellaneous", true);
    }
    
    void Loop()
    {
      if(inMainMenu)
        MainMenu();
      else
      {
        goToSubMenu();
      }

      delay(100);
    }

    void MainMenu()
    {
      lcd.setCursor(2, 0);
      lcd.print("Choose an option");

      // main menu items are hard-coded :)
      lcd.setCursor(0, 1);
      lcd.print("- Launch Control");

      lcd.setCursor(0, 2);
      lcd.print("- Trip Master");

      lcd.setCursor(0, 3);
      lcd.print("- Other");

      drawCursor();

      TM1638Banner("E.  ^v.3");
    }

    void goToSubMenu()
    {
      switch(currentSubMenu)
      {
        case 1:
          LaunchControl();
          break;
        case 2:
          TripMaster();
          break;
        case 3:
          void();
      }
    }

    void LaunchControl()
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
    }

    void TripMaster()
    {
      lcd.setCursor(4, 0);
      lcd.print("Trip Master");

      lcd.setCursor(5, 1);
      lcd.print("Distance:");

      lcd.setCursor(2, 2);
      lcd.print("Current    Total");

      lcd.setCursor(2, 3);
      lcd.print(String(currentDistance));

      lcd.setCursor(10, 3);
      lcd.print(String(totalDistance));
    }

    void ButtonReceiver(short button)
    {
      switch(button)
      {
        case 2:
          Back();
          break;
        case 16:
          changeCursorVertical(true); // up
          break;
        case 32:
          changeCursorVertical(false); // down
          break;
        case 64:
          Choose(); // choose / apply
          break;
      }
    }

    void drawCursor()
    {
      // x = 19 is occupied by the cursor, so can be cleared on every y.
      clearCursor();

      // draw cursor on current position
      lcd.setCursor(19, cursorY);
      lcd.print("*");
    }

    void clearCursor()
    {
      for(int y = 1; y < 4; y++)
      {
        if(y != cursorY)
        {
          lcd.setCursor(19, y);
          lcd.print(" ");
        } 
      }
    }

    void changeCursorVertical(bool up)
    {
      if(up)
      {
        cursorY -= 1;
        if(cursorY < 1) cursorY = 3;
      } else{
        cursorY += 1;
        if(cursorY > 3) cursorY = 1;
      }
    }

    void Back()
    {
      lcd.clear();
      reset();
    }

    void Choose()
    {
      lcd.clear();

      if(inMainMenu)
      {
        inMainMenu = false;
        currentSubMenu = cursorY;

        cursorY = 1;
      }
    }

    void reset()
    {
      inMainMenu = true;
      started = false;
      isPressedBefore = false;

      setRelays(false);
      SetVoltage(idleVoltage);

      setLEDs(false);
    }
};
