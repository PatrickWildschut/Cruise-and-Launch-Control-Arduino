#pragma once

// This file only exists for variables/functions that will be used across other files.

// 0: Read. 1: Cruise control, 2: Launch control
//template <class Mode>
Mode* Modes[4];
int CurrentMode = 0;

// Connected pins
const int ThrottleIn = A0;
const int ClutchIn = A1;
const int BrakeIn = A2;

const uint8_t Relay1 = 12;
const uint8_t Relay2 = 13;

void setRelays(bool set)
{
  digitalWrite(Relay1, !set);
  digitalWrite(Relay2, !set);
}
