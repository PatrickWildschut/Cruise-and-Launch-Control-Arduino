#pragma once
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 MCP4725;

bool SetThrottleByVoltage(float volt) {
    int toAnalogOut = volt * 880; // formula: volt * 1000 * 0.88;

    if (toAnalogOut > 4096) toAnalogOut = 4096;

    return MCP4725.setVoltage(toAnalogOut, false);
}

bool SetThrottleByPercentage(short percentage) {
    if (percentage < 0 || percentage > 100) {
        return 0;
    }

    return SetThrottleByVoltage(4.6 / (100.0 / (float)percentage));
}
