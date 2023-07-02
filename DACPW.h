#pragma once
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 MCP4725;

bool SetVoltage(float volt)
{
  int toAnalogOut = (volt * 1000) * 0.88;

  if(toAnalogOut > 4096) toAnalogOut = 4096;
  
  return MCP4725.setVoltage(toAnalogOut, false, 100000);
}
