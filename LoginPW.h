#pragma once

class Login : public Mode {
private:
    bool failed = false;
    byte pass[3];
    byte count = 0;

public:
    void Setup() {
        lcd.clear();
        SetThrottleByVoltage(idleVoltage);
        setRelays(true);

        setBanner("  Login to use car");
        TM1638Banner(" LOGIN");

        lcd.setCursor(7, 0);
        lcd.print("LOGIN");
    }

    void Loop() {

        if (count > 3) {
            // Wrong password
            lockCar();
            failed = true;
            return;
        }

        if (checkPass() && !failed) {
            LoggedIn = true;

            lcd.clear();
            TM1638.reset();

            reset();
            setRelays(false);
            SetThrottleByVoltage(idleVoltage);

            loggedInDemo();

            // comment this to skip checking connectivity.
            checkConnections();

            CurrentMode = 0;
            Modes[CurrentMode]->Setup();
            return;
        }
    }

    void ButtonReceiver(short button) {
        switch (button) {
        case 8:
            Trigger5();
            break;
        case 16:
            Trigger6();
            break;
        case 32:
            Trigger7();
            break;
        case 64:
            Trigger8();
            break;
        }
    }

    void Trigger5() {
        reset();
        print();
    }

    void Trigger6() {
        pass[count] = 1;
        count += 1;

        print();
    }

    void Trigger7() {
        pass[count] = 2;
        count += 1;

        print();
    }

    void Trigger8() {
        pass[count] = 3;
        count += 1;

        print();
    }

    void print() {
        LCDreset(1);

        String text = "";
        for (int i = 0; i < count; i++) {
            if (i > 2) break;
            text += "*  ";
        }
        lcd.setCursor(0, 1);
        lcd.print(text);
    }

    void lockCar() {
        // disable throttle
        SetThrottleByVoltage(0);

        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("   Login Failed!    ");
        lcd.setCursor(0, 2);
        lcd.print("   Car disabled.    ");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
    }

    bool checkPass() {
        return pass[0] == 2 && pass[1] == 1 &&
               pass[2] == 3;
    }

    void loggedInDemo() {
        LCDDemo();
        for (int i = 0; i < 7; i++) {
            demoLEDs();
        }

        lcd.clear();
    }

    void reset() {
        for (int i = 0; i < 3; i++) {
            pass[i] = 0;
        }

        count = 0;
    }

    void checkConnections() {
        bool failed;
        lcd.clear();

        do {
            failed = false;
            lcd.setCursor(1, 0);
            lcd.print("Throttle check...");
            lcd.setCursor(0, 1);

            if (GetThrottle() < 0.2) {
                lcd.print("[-] ERROR");
                failed = true;
            } else {
                lcd.print("[+] Connected");
            }

            lcd.setCursor(4, 2);
            lcd.print("DAC check...");
            lcd.setCursor(0, 3);

            if (SetThrottleByVoltage(1) == false) {
                lcd.print("[-] ERROR");
                failed = true;
            } else {
                lcd.print("[+] Connected");
            }

            delay(2000);
        } while (failed);
    }

};