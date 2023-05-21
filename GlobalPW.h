#pragma once

// This file only exists for variables/functions that will be used across other files.

// 0: Read. 1: Cruise control, 2: Launch control

bool LoggedIn = false;

Mode* Modes[5];
int CurrentMode = 4;

float idleVoltage = 0.6;

// Connected pins
const int ThrottleIn = A0;
const int ClutchIn = A1;
const int BrakeIn = A2;
const int SpeedIn = 12;

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

float GetSpeed()
{
  // wait for max      ->                  1 sec
  float duration = pulseIn(SpeedIn, HIGH, 1000000)  / 10000.0;

  // don't divide by 0
  if(duration == 0) return 0;

  // self made formula :)
  return (1 / duration) * 37;
}

bool ThrottlePressed()
{
  if(analogRead(ThrottleIn) * 5.0 / 1023.0 > 1.2)
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
