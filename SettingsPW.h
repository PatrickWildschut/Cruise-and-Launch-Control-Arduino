#pragma once

class Settings : public Mode {
private:
    byte cursorY = 1;
    bool inMainMenu = true;
    byte currentSubMenu = 0;

public:
    void Setup() {
        setRelays(false);
        SetThrottleByVoltage(idleVoltage);

        ShowBanner("      Settings", false);

        inMainMenu = true;
    }

    void Loop() {
        if (inMainMenu)
            MainMenu();
        else {
            goToSubMenu();
        }

        delay(50);
    }

    void MainMenu() {
        lcd.setCursor(2, 0);
        lcd.print("Choose an option");

        // main menu items are hard-coded :)
        lcd.setCursor(0, 1);
        lcd.print("- Layouts");

        lcd.setCursor(0, 2);
        lcd.print("- Other");

        lcd.setCursor(0, 3);
        lcd.print("- Other");

        drawCursor();

        TM1638Banner("E.  ^v.3");
    }

    void goToSubMenu() {
        switch (currentSubMenu) {
        case 1:
            Layouts();
        case 2:
            void();
        case 3:
            void();
        }
    }

    void Layouts() {
        TM1638Banner("E.<>^v.3");

        lcd.setCursor(0, 0);
        lcd.print("Choose Mode Layouts");

        lcd.setCursor(0, 1);
        lcd.print("Read  : " + String(ReadModeLayout == 0 ? "Horizontal" : "Vertical  "));

        lcd.setCursor(0, 2);
        lcd.print("Cruise: " + String(CruiseModeLayout == 0 ? "Horizontal" : "Vertical  "));

        lcd.setCursor(0, 3);
        lcd.print("Rally : " + String(MiscModeLayout == 0 ? "Horizontal" : "Vertical  "));

        drawCursor();
    }

    void ButtonReceiver(short button) {
        switch (button) {
        case 2:
            Back(); // back
            break;
        case 4:
            changeCursorHorizontal(false); // left
            break;
        case 8:
            changeCursorHorizontal(true); // right
            break;
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

    void changeCursorHorizontal(bool right) {
        if (inMainMenu) return;

        // Layouts
        if (currentSubMenu == 1) {
            // change the value of current selected mode
            // cursorY = 1 -> Read mode layout
            // cursorY = 2 -> Cruise mode layout
            // cursorY = 3 -> Misc mode layout
            switch (cursorY) {
            case 1:
                ReadModeLayout = !ReadModeLayout;
                break;
            case 2:
                CruiseModeLayout = !CruiseModeLayout;
                break;
            case 3:
                MiscModeLayout = !MiscModeLayout;
                break;
            }
        }
    }

    void Back() {
        lcd.clear();
        inMainMenu = true;
    }

    void Choose() {
        lcd.clear();

        if (inMainMenu) {
            inMainMenu = false;
            currentSubMenu = cursorY;

            cursorY = 1;

            return;
        }

        // Save Layouts
        if (currentSubMenu == 1) {
            // cursorY = 1 -> Read mode layout
            // cursorY = 2 -> Cruise mode layout
            // cursorY = 3 -> Misc mode layout
            switch (cursorY) {
            case 1:
                EEPROM.write(0, ReadModeLayout);
                break;
            case 2:
                EEPROM.write(1, CruiseModeLayout);
                break;
            case 3:
                EEPROM.write(2, MiscModeLayout);
                break;
            }
        }
    }
};
