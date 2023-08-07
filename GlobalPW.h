#pragma once

// This file only exists for variables/functions that will be used across other files.

// 0: Read. 1: Cruise control, 2: Launch control

bool LoggedIn = false;

Mode* Modes[5];
short CurrentMode = 4; // default to login mode, mode 4 because counting from 0, aka login mode = 5
short TotalModes = 3; // login mode doesnt count as a mode.

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
  // wait for max      ->                  0.125 sec -> otherwise it will return 0
  float duration = pulseIn(SpeedIn, HIGH, 125000)  / 10000.0;

  // waited too long, but don't divide by 0
  if(duration == 0) return 0;

  // self made formula :)
  return (1 / duration) * 38;
}

float GetAcceleration()
{
  unsigned long deltaTime = millis();
  float deltaSpeed = GetSpeed() / 3.6;

  delay(50);

  deltaTime = millis() - deltaTime;
  deltaSpeed = GetSpeed() / 3.6 - deltaSpeed;

  return deltaSpeed / (deltaTime / 1000.0);
}

float GetGForce()
{
  return GetAcceleration() / 9.81;
}

bool ThrottlePressed()
{
  return analogRead(ThrottleIn) > 205;
}

bool ThrottlePressed(float minVolt)
{
  return analogRead(ThrottleIn) * 5.0 / 1023.0 > minVolt;
}

bool ClutchPressed()
{
  return analogRead(ClutchIn) < 205;
}

bool BrakePressed()
{
  return analogRead(BrakeIn) > 205;
}
