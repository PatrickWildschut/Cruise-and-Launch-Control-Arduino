#pragma once

// This file only exists for variables/functions that will be used across other files.

// 0: Read. 1: Cruise control, 2: Misc 3: Settings 4: About 5: Login

const char *currentVersion = "Version: 1.0.0";

bool LoggedIn = false;

Mode* Modes[6];
short CurrentMode = 5; // default to login mode, mode 5 because counting from 0, aka login mode = 6
const byte TotalModes = 4; // login mode doesnt count as a mode.

Mode* MiscSubModes[3];

const float idleVoltage = 0.0;

// Connected pins
const byte ThrottleIn = A0;
const byte ClutchIn = A1;
const byte BrakeIn = A2;
const byte SpeedIn = 12;

const byte Relay1 = 8;
const byte Relay2 = 9;

// Layouts
byte ReadModeLayout = 0;
byte CruiseModeLayout = 0;
byte MiscModeLayout = 0;

void LoadLayouts() {
    /* the EEPROM is used to write and read last chosen layouts.
     i have set the EEPROM up like this:
     0: Read Mode Layout -> 0: Horizontal 1: Vertical
     1: Cruise Control Mode Layout -> 0: Horizontal 1: Vertical
     2: Miscellaneous Mode Layout -> 0: Horizontal 1: Vertical
    */

    ReadModeLayout = EEPROM.read(0);
    CruiseModeLayout = EEPROM.read(1);
    MiscModeLayout = EEPROM.read(2);
}

void setRelays(bool set) {
    digitalWrite(Relay1, !set);
    digitalWrite(Relay2, !set);
}

float GetThrottle() {
    return analogRead(ThrottleIn) * 0.0049; // formula: throttle * 5.0 / 1023.0
}
short GetThrottlePercentage() {
    float throttle = GetThrottle() - 0.6;
    int percentage = throttle * 25.0; // formula: throttle / 4.0 * 100.0

    if (percentage <= 0) return 0;

    if (percentage >= 100) return 100;

    return percentage;
}

float GetSpeed() {
    // wait for max      ->                  0.125 sec -> otherwise it will return 0
    float duration = pulseIn(SpeedIn, HIGH, 125000) / 10000.0;

    // waited too long, but don't divide by 0
    if (duration == 0) return 0;

    // self made formula :)
    return (1 / duration) * 38;
}

float GetDistance() {
    float speed = GetSpeed() / 3.6;
    delay(10);

    return speed / 5.8;
}

String PercentageToText(short percentage) {
    if (percentage < 10) {
        return String(percentage) + "  ";
    }

    return String(percentage) + " ";
}

bool ThrottlePressed() {
    return analogRead(ThrottleIn) > 205;
}

bool ThrottlePressed(float minVolt) {
    return analogRead(ThrottleIn) * 0.0049 > minVolt;
}

bool ClutchPressed() {
    return analogRead(ClutchIn) < 205;
}

bool BrakePressed() {
    return analogRead(BrakeIn) > 205;
}

/* This function returns an approximate RPM for all gears
   based on car speed.

   gear: current gear, 1-5
   Out: RPM
*/
int GetRPMBasedOnSpeed(int gear) {

    float speed = GetSpeed() - 3; // -3 offset
    int RPM;

    switch (gear) {
    case 1:
        RPM = 156 * speed;
        break;
    case 2:
        RPM = 86 * speed;
        break;
    case 3:
        RPM = 57 * speed;
        break;
    case 4:
        RPM = 43 * speed;
        break;
    case 5:
        RPM = 34.5 * speed;
        break;
    default:
        RPM = 700;
        break;
    }

    if (RPM < 700) RPM = 700;

    return RPM;
}

int GetRPM() {
    // Can't wait until I can code this
    // Have fun coding future Patrick :)
    return 0;
}
