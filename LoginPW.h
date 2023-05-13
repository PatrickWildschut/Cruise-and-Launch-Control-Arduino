class Login : public Mode
{
  private:
    short* pass[3];
    short count = 0;

  public:
    void Setup()
    {
      setBanner("  Login to use car");
    }

    void Loop()
    {
      lcd.setCursor(7, 0);
      lcd.print("LOGIN");

      if(count > 3)
      {
        // disable throttle
        SetVoltage(0);

        lcd.setCursor(3, 1);
        lcd.print("Login Failed!");
        lcd.setCursor(3, 2);
        lcd.print("Car disabled.");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        return;
      }

      if(checkPass())
      {
        CurrentMode = 0;
        Modes[CurrentMode]->Setup();
        return;
      }
    }

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
        text += "*  ";
      }
      lcd.setCursor(0, 1);
      lcd.print(text);
    }

    bool checkPass()
    {
      return pass[0] == 2 && pass[1] == 1 &&
            pass[2] == 3;
    }
};