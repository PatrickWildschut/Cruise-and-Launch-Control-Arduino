#pragma once

class Mode {
public:
    virtual Mode() {}
    virtual void Setup();
    virtual void Loop();

    virtual void ButtonReceiver(short button);

    void ShowBanner(String text, boolean showModeText) {
        lcd.clear();
        TM1638.reset();

        lcd.setCursor(0, 1);
        lcd.print(text);

        if (showModeText) {
            lcd.setCursor(8, 2);
            lcd.print("Mode");
        }

        demoLEDs();
        demoLEDs();

        lcd.clear();
    }
};
