class Login : public Mode
{
  private:
    short* pass[3];
    short count = 0;

  public:
    void Setup()
    {
      SetVoltage(idleVoltage);

      setBanner("  Login to use car");
    }

    void Loop()
    {
      lcd.setCursor(7, 0);
      lcd.print("LOGIN");

      if(count > 3)
      {
        // Wrong password
        lockCar();
        return;
      }

      if(checkPass())
      {
        LoggedIn = true;

        lcd.clear();
        TM1638.reset();

        setRelays(false);
        SetVoltage(idleVoltage);

        loggedInDemo();

        CurrentMode = 0;
        Modes[CurrentMode]->Setup();
        return;
      }
    }

    void Trigger5(){}

    void Trigger6()
    {
      pass[count] = 1;
      count += 1;

      print();
    }

    void Trigger7()
    {
      pass[count] = 2;
      count += 1;

      print();
    }

    void Trigger8()
    {
      pass[count] = 3;
      count += 1;

      print();
    }

    void print()
    {

      String text = "";
      for(int i = 0; i < count; i++)
      {
        if(i > 2) break;
        text += "*  ";
      }
      lcd.setCursor(0, 1);
      lcd.print(text);
    }

    void lockCar()
    {
      // disable throttle, 0 volt should cause an error on the instrument cluster
      SetVoltage(0);

      lcd.setCursor(3, 1);
      lcd.print("Login Failed!");
      lcd.setCursor(3, 2);
      lcd.print("Car disabled.");
      lcd.setCursor(0, 3);
      lcd.print("--------------------");
    }

    bool checkPass()
    {
      return pass[0] == 2 && pass[1] == 1 &&
            pass[2] == 3;
    }

    void loggedInDemo()
    {
      LCDDemo();
      for(int i = 0; i < 7; i++)
      {
        demoLEDs();
      }

      lcd.clear();
    }

};