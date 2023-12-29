#pragma once

class Read : public Mode {
private:
    // current mode within read mode
    short readIndex = 0;
    const short totalModes = 2;
    const short recommendedRPM = 2000;

public:
    void Setup() {
        setRelays(false);
        SetThrottleByVoltage(idleVoltage);

        readIndex = 0;

        ShowBanner("        Read", true);

        TM1638Banner("E.    .3");
    }

    void Loop() {
        switch (readIndex) {
        case 0:
            Default();
            break;
        case 1:
            recommendedGear();
            break;
        }

        if (BrakePressed()) {
            LEDsOnBrake(true);
        } else {
            LEDsOnBrake(false);
            LEDsBasedOnPercentage(GetThrottlePercentage());
        }

        delay(10);
    }

    void ButtonReceiver(short button) {
        switch (button) {
        case 2:
            ReadBack();
            break;
        case 64:
            ReadNext();
            break;
        }
    }

    void ReadBack() {
        lcd.clear();
        readIndex -= 1;

        if (readIndex < 0) readIndex = totalModes - 1;
    }

    void ReadNext() {
        lcd.clear();
        readIndex += 1;

        if (readIndex > totalModes - 1) readIndex = 0;
    }

    void Default() {
        ReadModeLayout == 0 ? Horizontal() : Vertical();
    }

    void Horizontal() {
        short percentage = GetThrottlePercentage();

        // Display to I2C
        lcd.setCursor(0, 0);
        lcd.print("Throttle:     " + PercentageToText(percentage));
        lcd.setCursor(17, 0);
        lcd.print("%");

        lcd.setCursor(0, 1);
        lcd.print("Clutch:       " + String(ClutchPressed() ? "++++" : "    "));

        lcd.setCursor(0, 2);
        lcd.print("Brake:        " + String(BrakePressed() ? "++++" : "    "));

        lcd.setCursor(0, 3);
        lcd.print("Speed:        " + String(GetSpeed()));
        //TM1638.displayText("    " + String(int(GetSpeed())));
    }

    void Vertical() {
        short percentage = GetThrottlePercentage();

        // Display to I2C
        lcd.setCursor(1, 0);
        lcd.print("Throttle    Speed");

        lcd.setCursor(4, 1);
        lcd.print(PercentageToText(percentage));

        lcd.setCursor(7, 1);
        lcd.print("%");

        lcd.setCursor(13, 1);
        lcd.print(String(GetSpeed()));

        lcd.setCursor(2, 2);
        lcd.print("Clutch     Brake");

        lcd.setCursor(2, 3);
        lcd.print(String(ClutchPressed() ? "++++++" : "      "));

        lcd.setCursor(13, 3);
        lcd.print(String(BrakePressed() ? "+++++" : "     "));

    }

    void recommendedGear() {
        int gear;

        for (gear = 2; gear <= 5; gear++) {
            if (GetRPM(gear) < recommendedRPM) {
                break;
            }
        }

        lcd.setCursor(1, 0);
        lcd.print("Recommended Gears");

        lcd.setCursor(0, 2);
        lcd.print(GearToString(gear - 1) + String(GetRPM(gear - 1)));
        lcd.setCursor(0, 3);
        lcd.print(GearToString(gear) + String(GetRPM(gear)));
    }

    String GearToString(int gear) {

        switch (gear) {
        case 1:
            return "    First:  ";
        case 2:
            return "    Second: ";
        case 3:
            return "    Third:  ";
        case 4:
            return "    Fourth: ";
        case 5:
            return "    Fifth:  ";
        }

        return "";
    }
};
