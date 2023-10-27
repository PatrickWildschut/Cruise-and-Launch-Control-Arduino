#pragma once

class Misc : public Mode {
private:
    bool inMainMenu = true;
    byte cursorY = 1;
    byte currentSubMode = 0;

public:
    void Setup() {
        setRelays(false);
        SetThrottleByVoltage(idleVoltage);

        reset();

        ShowBanner("   Miscellaneous", false);
    }

    void Loop() {
        if (inMainMenu)
            MainMenu();
        else {
            goToSubModes();
        }
    }

    void MainMenu() {
        lcd.setCursor(2, 0);
        lcd.print("Choose an option");

        // main menu items are hard-coded :)
        lcd.setCursor(0, 1);
        lcd.print("- Launch Control");

        lcd.setCursor(0, 2);
        lcd.print("- Rev Test");

        lcd.setCursor(0, 3);
        lcd.print("- Trip Master");

        drawCursor();

        TM1638Banner("E   ^v.3");

        delay(100);
    }

    void goToSubModes() {
        MiscSubModes[currentSubMode]->Loop();
    }

    /* Triggered in main loop of program

    */
    void ButtonReceiver(short button) {
        if (!inMainMenu) {

            // check if user pressed button 2, go back to main menu
            if (button == 2) {
                Back();
                return;
            }

            MiscSubModes[currentSubMode]->ButtonReceiver(button);
            return;
        }

        switch (button) {
        case 16:
            changeCursorVertical(true); // up
            break;
        case 32:
            changeCursorVertical(false); // down
            break;
        case 64:
            Choose(); // choose / apply
            break;
        }
    }

    void drawCursor() {
        // x = 19 is occupied by the cursor, so can be cleared on every y.
        clearCursor();

        // draw cursor on current position
        lcd.setCursor(19, cursorY);
        lcd.print("*");
    }

    void clearCursor() {
        for (int y = 1; y < 4; y++) {
            if (y != cursorY) {
                lcd.setCursor(19, y);
                lcd.print(" ");
            }
        }
    }

    void changeCursorVertical(bool up) {
        if (up) {
            cursorY -= 1;
            if (cursorY < 1) cursorY = 3;
        } else {
            cursorY += 1;
            if (cursorY > 3) cursorY = 1;
        }
    }

    void Back() {
        lcd.clear();
        reset();
    }

    void Choose() {
        lcd.clear();

        inMainMenu = false;
        currentSubMode = cursorY - 1;

        MiscSubModes[currentSubMode]->Setup();

        cursorY = 1;
    }

    void reset() {
        inMainMenu = true;

        setRelays(false);
        SetThrottleByVoltage(idleVoltage);

        setLEDs(false);
    }
};
