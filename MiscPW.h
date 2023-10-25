#pragma once

#include "MiscTripMaster.h"
#include "MiscLaunchControl.h"
#include "MiscRally.h"

class Misc : public Mode {
private:
    bool inMainMenu = true;
    byte cursorY = 1;
    byte currentSubMenu = 0;

public:
    void Setup() {
        setRelays(false);
        SetVoltage(idleVoltage);

        reset();

        ShowBanner("   Miscellaneous", false);
    }

    void Loop() {
        if (inMainMenu)
            MainMenu();
        else {
            goToSubMenu();
        }
    }

    void MainMenu() {
        lcd.setCursor(2, 0);
        lcd.print("Choose an option");

        // main menu items are hard-coded :)
        lcd.setCursor(0, 1);
        lcd.print("- Launch Control");

        lcd.setCursor(0, 2);
        lcd.print("- Trip Master");

        lcd.setCursor(0, 3);
        lcd.print("- Rally / Track");

        drawCursor();

        TM1638Banner("E.  ^v.3");

        delay(100);
    }

    void goToSubMenu() {
        switch (currentSubMenu) {
        case 1:
            LaunchControl();
            break;
        case 2:
            TripMaster();
            break;
        case 3:
            Rally();
            break;
        }
    }

    void ButtonReceiver(short button) {
        switch (button) {
        case 2:
            Back();
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

    void Back() {
        lcd.clear();
        reset();
    }

    void Choose() {
        lcd.clear();

        if (inMainMenu) {
            inMainMenu = false;
            currentSubMenu = cursorY;

            cursorY = 1;

            return;
        }

        // not in menu
        switch (currentSubMenu) {
        // Launch control
        case 1:
            LaunchControlChoose();
            break;
        // Trip master
        case 2:
            TripMasterChoose();
            break;

        case 3:
            RallyChoose();
            break;
        }
    }

    void reset() {
        inMainMenu = true;
        started = false;
        isPressedBefore = false;

        setRelays(false);
        SetVoltage(idleVoltage);

        setLEDs(false);
    }
};
