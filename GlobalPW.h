#pragma once

// This file only exists for variables/functions that will be used across other files.

// 0: Read. 1: Cruise control, 2: Launch control
//template <class Mode>
Mode* Modes[5];
int CurrentMode = 4;

// Connected pins
const int ThrottleIn = A0;
const int ClutchIn = A1;
const int BrakeIn = A2;

const uint8_t Relay1 = 8;
const uint8_t Relay2 = 9;

void setRelays(bool set)
{
  digitalWrite(Relay1, !set);
  digitalWrite(Relay2, !set);
}

float GetThrottle()
{
  return analogRead(ThrottleIn) * 5.0 / 1023.0;
}

bool ThrottlePressed()
{
  if(analogRead(ThrottleIn) * 5.0 / 1023.0 > 1)
  {
    return true;
  }

  return false;
}

bool ClutchPressed()
{
  if(analogRead(ClutchIn) * 5.0 / 1023.0 < 1)
  {
    return true;
  }

  return false;
}

bool BrakePressed()
{
  if(analogRead(BrakeIn) * 5.0 / 1023.0 > 1)
  {
    return true;
  }

  return false;
}

