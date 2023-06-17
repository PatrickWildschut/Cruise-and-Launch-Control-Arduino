class Settings : public Mode
{
  
  public:
    void Setup()
    {
      setRelays(false);
      SetVoltage(idleVoltage); 
    
      ShowBanner("      Settings", false);
    }
    
    void Loop()
    {
      lcd.setCursor(0, 0);
      lcd.print("To be made...");

      delay(100);
    }

    void Trigger5(){}

    void Trigger6(){}

    void Trigger7(){}

    void Trigger8(){}
};
