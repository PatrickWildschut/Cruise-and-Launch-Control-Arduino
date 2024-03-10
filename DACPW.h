#pragma once
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 MCP4725;

bool SetThrottleByVoltage(float volt) {
  // 0.25 is the throttle offset between the DAC and actual throttle, don't worry abt it
    int toAnalogOut = (volt - 0.25) * 880; // formula: volt * 1000 * 0.88;

    if (toAnalogOut > 4096) toAnalogOut = 4096;
    
    if (toAnalogOut < 0) toAnalogOut = 0;

    return MCP4725.setVoltage(toAnalogOut, false); 
}

bool SetThrottleByPercentage(short percentage) {
    if (percentage < 0 || percentage > 100) {
        return 0;
    }

    return SetThrottleByVoltage(4.6 / (100.0 / (float)percentage));
}
