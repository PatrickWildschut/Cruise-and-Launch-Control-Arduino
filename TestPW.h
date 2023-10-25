class Test : public Mode {

public:
    void Setup() {
        SetVoltage(idleVoltage);
        setRelays(false);

        CurrentMode = 3;
        lcd.clear();

        lcd.setCursor(0, 0);
        lcd.print("TESTING SPEED");

        setBanner("Current mode: Test");
    }

    void Loop() {
        float duration = pulseIn(SpeedIn, HIGH, 5000000) / 10000.0;
        float thing = 0;

        if (duration != 0) {
            thing = (1 / duration) * 36;
        }

        lcd.setCursor(0, 0);
        lcd.print(String(duration));

        lcd.setCursor(0, 2);
        lcd.print(String(thing));
    }

    void Trigger5() {}

    void Trigger6() {

    }

    void Trigger7() {
    }

    void Trigger8() {
    }
};
