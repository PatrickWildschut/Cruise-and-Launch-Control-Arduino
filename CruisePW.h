#pragma once

class Cruise : public Mode {
private:
    const byte minSpeed = 30;
    const float baseVoltage = 1.10;
    const byte maxSpeedDifference = 10; // Tenth of maximum difference between desired speed and current speed before changing voltage. 10 = 1
    const float voltageIntervene = 0.01;

    bool enabled = false;

    float currentVoltage = 0;
    float oldSetThrottleByVoltage = 0;

    float currentSpeed = 0;
    float oldSpeed;
    short desiredSpeed = -1;
    bool update;

    bool increaseDesiredSpeed = false;
    byte speedChangeFeedbackIndex = 0;

public:
    void Setup() {

        setRelays(false);
        SetThrottleByVoltage(idleVoltage);

        enabled = false;
        reset();

        ShowBanner("   Cruise Control", true);
    }

    void Loop() {
        lcd.setCursor(1, 0);

        // update current speed
        currentSpeed = GetSpeed();

        if (!enabled) {
            lcd.print("Currently disabled");

            if (desiredSpeed != -1) {
                lcd.setCursor(2, 2);
                lcd.print("Saved speed:  " + String(desiredSpeed));
            }

            TM1638Banner("Eno  <>3");

            setRelays(false);
            setLEDs(false);
            return;
        }

        // cruise control is enabled

        // choose layout
        CruiseModeLayout == 0 ? Horizontal() : Vertical();

        walkingLEDs();
        updateTM1638();
        checkPedalsPressed();

        update ? controlSpeed() : void();
        update = !update;

        delay(100);
    }

    void ButtonReceiver(short button) {
        if (enabled) {
            switch (button) {
            case 2:
                enabled = false;
                reset();
                break;
            case 16:
                ChangeDesiredSpeed(-1, false);
                break;
            case 32:
                ChangeDesiredSpeed(1, false);
                break;
            case 64:
                ChangeDesiredSpeed(5, true);
                break;
            }
        } else {
            // not enabled, buttons:
            switch (button) {
            case 2:
                EnableCruise(true);
                break;
            case 4:
                EnableCruise(false);
                break;
            case 32:
                ChangeDesiredSpeed(-5, true);
                break;
            case 64:
                ChangeDesiredSpeed(5, true);
                break;
            }
        }

    }

    void EnableCruise(bool newSpeed) {
        enabled = true;
        currentVoltage = GetThrottle();

        // keep old speed if that speed isn't below the minSpeed
        if (desiredSpeed <= minSpeed || newSpeed)
            setCurrentSpeed();
    }

    void ChangeDesiredSpeed(int amount, bool snap) {
        bool positive = amount > 0 ? true : false;

        // show on TM1638 display little animation
        speedChangeFeedbackIndex = 5;
        increaseDesiredSpeed = positive;

        if (snap) {
            int remainder = desiredSpeed % amount;

            desiredSpeed += amount - remainder;
        } else {
            desiredSpeed += amount;
        }

        if (desiredSpeed < minSpeed) desiredSpeed = minSpeed;

    }

    void controlSpeed() {
        setNewVoltage(); // sets currentVoltage

        // only update DAC if there is a difference to avoid EAC fail
        if (currentVoltage != oldSetThrottleByVoltage) {
            SetThrottleByVoltage(currentVoltage);

            oldSetThrottleByVoltage = currentVoltage;
        }

        oldSpeed = currentSpeed;
    }

    void updateTM1638() {
        // if speed changed, show arrow on TM1638 and feedback on LCD for `speedChangeFeedbackIndex` amount of time
        if (speedChangeFeedbackIndex > 0) {
            speedChangeFeedback();
        } else {
            TM1638Banner("E0  <>>3");
        }
    }

    void checkPedalsPressed() {
        // clutch or brake are pressed, if so, reset
        if (CBPressed()) {
            enabled = false;
            reset();
            return;
        }

        // Check if throttle is pressed in more than the voltage we are providing
        // if so, switch to physical pedal, otherwise, just keep cruise control enabled :)
        bool enabled = !ThrottlePressed(currentVoltage - 0.3);

        setRelays(enabled);

        lcd.setCursor(0, 0);
        if (enabled) {
            lcd.print(" Currently ENABLED ");
        } else {
            lcd.print(" Release Throttle..");
        }

    }

    void reset() {
        lcd.clear();
        update = false;
        setRelays(false);
        currentVoltage = idleVoltage;
        SetThrottleByVoltage(idleVoltage);
    }

    bool CBPressed() {
        return ClutchPressed() || BrakePressed();
    }

    void speedChangeFeedback() {
        TM1638arrowDisplay(increaseDesiredSpeed);
        speedChangeFeedbackIndex -= 1;
    }

    void setNewVoltage() {

        // Compare speeds
        float desiredDifference = desiredSpeed - currentSpeed;
        float deltaSpeed = oldSpeed - currentSpeed;

        bool tooSlow = desiredDifference < -maxSpeedDifference * 0.1;
        bool tooFast = desiredDifference > maxSpeedDifference * 0.1;

        if (tooSlow) {
            if (deltaSpeed <= -desiredDifference * 0.1) {
                currentVoltage += desiredDifference * voltageIntervene;
            } else {
                currentVoltage -= desiredDifference * voltageIntervene;
            }
        } else if (tooFast) {
            if (deltaSpeed >= -desiredDifference * 0.1) {
                currentVoltage += desiredDifference * voltageIntervene;
            } else {
                currentVoltage -= desiredDifference * voltageIntervene;
            }
        } else {
            if (deltaSpeed < -0.1 || deltaSpeed > 0.1) {
                currentVoltage += desiredDifference * voltageIntervene;
            }
        }

        if (currentVoltage > 2) {
            currentVoltage = 2;
        } else if (currentVoltage < 0.5) {
            currentVoltage = 0.5;
        }

        // desiredDifference < 0 && deltaSpeed > 0 => Aan het klimmen naar desired

        // desiredDifference > 0 && deltaSpeed < 0 => Aan het dalen naar desired
    }

    void Horizontal() {
        lcd.setCursor(0, 1);
        lcd.print("Desired speed: " + String(desiredSpeed));

        lcd.setCursor(0, 2);
        lcd.print("Current speed:" + String(currentSpeed));
    }

    void Vertical() {
        lcd.setCursor(7, 1);
        lcd.print("Speed");

        lcd.setCursor(0, 2);
        lcd.print(" Volt     Delta V  ");

        lcd.setCursor(2, 3);
        lcd.print(String(currentVoltage));

        lcd.setCursor(13, 3);
        lcd.print(String(oldSpeed - currentSpeed));
    }

    void setCurrentSpeed() {
        // set to current speed based on vehicle speed
        float speed = GetSpeed();

        if (speed >= minSpeed) {
            desiredSpeed = speed;
        } else {
            enabled = false;
            reset();
        }
    }
};
