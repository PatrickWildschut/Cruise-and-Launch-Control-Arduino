#pragma once

class Mode
{
  public:
    virtual Mode() {}
    virtual void Setup();
    virtual void Loop();

    virtual void Trigger5();
    virtual void Trigger6();
    virtual void Trigger7();
    virtual void Trigger8();

    void ShowBanner(String text, boolean showModeText)
    {
       lcd.clear();
       
       lcd.setCursor(0, 1);
       lcd.print(text);

       if(showModeText)
       {
        lcd.setCursor(8, 2);
        lcd.print("Mode");
       }

       demoLEDs();
       demoLEDs();

       lcd.clear();
    }
};
