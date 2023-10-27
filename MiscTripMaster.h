#pragma once

// Trip master functions from within Misc
class TripMaster : public Mode {
private:
    float totalDistance = 0;
    float currentDistance;

public:
    void Setup() {
        currentDistance = 0;
    }

    void Loop() {
        float distance = GetDistance();
        currentDistance += distance;
        totalDistance += distance;

        LCDTripMaster();

        TM1638Banner("E.    .3");

        delay(50);
    }

    void ButtonReceiver(short button) {
        if (button == 64)
            currentDistance = 0;
    }

private:
    void TripMasterCurrent() {
        lcd.setCursor(3, 3);

        if (currentDistance < 1000) {
            lcd.print(String(int(currentDistance)));

            lcd.setCursor(7, 3);
            lcd.print("m  ");
        } else {
            lcd.print(String(currentDistance / 1000.0));

            lcd.setCursor(7, 3);
            lcd.print("km");
        }

    }

    void TripMasterTotal() {
        lcd.setCursor(13, 3);

        if (totalDistance < 1000) {
            lcd.print(String(int(totalDistance)));

            lcd.setCursor(18, 3);
            lcd.print("m ");
        } else {
            lcd.print(String(totalDistance / 1000.0));

            lcd.setCursor(18, 3);
            lcd.print("km");
        }
    }

    void LCDTripMaster() {
        lcd.setCursor(4, 0);
        lcd.print("Trip Master");

        lcd.setCursor(6, 1);
        lcd.print("Distance");

        lcd.setCursor(2, 2);
        lcd.print("Current    Total");

        TripMasterCurrent();
        TripMasterTotal();
    }
};
