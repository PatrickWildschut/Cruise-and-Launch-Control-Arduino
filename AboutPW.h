class About : public Mode
{
  
  public:
    void Setup()
    {
      setRelays(false);
      SetVoltage(idleVoltage); 
    
      ShowBanner("       About", false);
    }
    
    void Loop()
    {
      lcd.setCursor(7, 0);
      lcd.print("Author:");

      lcd.setCursor(2, 1);
      lcd.print("Patrick Wildschut");

      lcd.setCursor(3, 2);
      lcd.print("Copyright 2023");

      lcd.setCursor(3, 3);
      lcd.print("Version: 0.8.1");

      walkingLEDs();

      delay(100);
    }

    void Trigger5(){}

    void Trigger6(){}

    void Trigger7(){}

    void Trigger8(){}
};
