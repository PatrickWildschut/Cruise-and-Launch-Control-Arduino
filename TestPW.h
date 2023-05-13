class Test : public Mode
{
  private:
    float volt = 0.5;
  
  public:
    void Setup()
    {
      CurrentMode = 3;
      setRelays(true);

      volt = 0.5;

      lcd.setCursor(0, 0);
      lcd.print("TESTING VOLTAGES");
    
      setBanner("Current mode: Test");
    }
    
    void Loop()
    {
      bool success = SetVoltage(volt);
    
      if (success) 
      {
        TM1638.displayText(String(volt / 2) + String(volt));

        lcd.setCursor(3, 2);
        lcd.print("DAC Connected!");
      }
      else 
      {
        TM1638.displayText("Failed!");

        lcd.setCursor(1, 2);
        lcd.print("DAC Disconnected!");
      }
    }

    void Trigger6()
    {
      
    }

    void Trigger7()
    {
      volt *= 0.9;

      if(volt < 0) volt = 0;
    }

    void Trigger8()
    {
      volt *= 1.1;

      if(volt > 5) volt = 5;
    }
};
