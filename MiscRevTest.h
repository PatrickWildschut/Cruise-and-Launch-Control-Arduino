#pragma once

// Within Misc
class RevTest : public Mode {

private:
    byte throttleRange;

    long revStart;
    short revMilliseconds;

    long waitStart;
    short waitMilliseconds;

    bool enabled;
    bool isRevving;
    byte throttlePercentage;

public:
    void Setup() {
        reset();

        ShowBanner("      Rev Test", true);
    }

    void Loop() {

        if (enabled) {
            Rev();
        } else {
            LCDShowDisabled();
        }

        TM1638Banner("E.  un.3");

        delay(50);
    }

    void ButtonReceiver(short button) {
        switch (button) {
        case 16:
            AdjustRange(-5);
            break;
        case 32:
            AdjustRange(5);
            break;

        case 64:
            SetEnabled();
            break;
        }
    }

private:
    void LCDShowDisabled() {
        lcd.setCursor(1, 0);
        lcd.print("Throttle Percentage");

        lcd.setCursor(5, 2);
        lcd.print(PercentageToText(throttleRange - 10)  + "% - "
                  + PercentageToText(throttleRange + 10) + "%");
    }

    void LCDShowEnabled(long delay) {
        lcd.setCursor(4, 0);
        int diff;

        if (isRevving) {
            lcd.print("Revving...");
            diff = revMilliseconds - delay;

            lcd.setCursor(3, 1);
            lcd.print("Throttle: " + String(throttlePercentage) + "%");
        } else {
            lcd.print("Waiting...");
            diff = waitMilliseconds - delay;
        }

        lcd.setCursor(3, 3);
        lcd.print("Timeout: ");
        lcd.setCursor(12, 3);

        lcd.print(diff);
    }

    void AdjustRange(short amount) {
        throttleRange += amount;

        if (throttleRange < 10) throttleRange = 10;

        if (throttleRange > 90) throttleRange = 90;
    }

    void SetEnabled() {
        enabled = !enabled;
        lcd.clear();

        // disable relays
        if (!enabled) {
            setRelays(false);
            SetThrottleByVoltage(idleVoltage);
        } else {
            // enable relays
            setRelays(true);
            waitStart = millis();
        }
    }

    void Rev() {
        if (!isRevving) {
            long waitDelay = millis() - waitStart;

            // delay between throttles, no throttle
            if (waitDelay < waitMilliseconds) {
                LCDShowEnabled(waitDelay);
                LCDreset(1);
                return;
            }

            // waiting done, set throttle
            throttlePercentage = random(throttleRange - 10, throttleRange + 10);
            SetThrottleByPercentage(throttlePercentage);

            // set timeout
            if (throttlePercentage >= 30) {
                revMilliseconds = 500;
            } else {
                revMilliseconds = 800;
            }
            revStart = millis();
            isRevving = true;
        }
        // revving

        long revDelay = millis() - revStart;
        // revMilliseconds passed, default throttle value
        if (revDelay < revMilliseconds) {
            LCDShowEnabled(revDelay);
            return;
        }

        waitStart = millis();
        SetThrottleByVoltage(idleVoltage);
        isRevving = false;

    }

    void reset() {
        throttleRange = 20;
        revMilliseconds = 1000;
        waitMilliseconds = 5000;
        isRevving = false;

        setRelays(false);
        SetThrottleByVoltage(idleVoltage);

        enabled = false;
    }
};
