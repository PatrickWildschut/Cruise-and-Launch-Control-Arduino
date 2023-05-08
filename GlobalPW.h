#pragma once

// This file only exists for variables/functions that will be used across other files.

LiquidCrystal_I2C lcd(0x27, 20, 4);

// Connected pins
const int Throttle3 = A0;
const int Throttle6 = A1;

void setBanner(String message)
{
  lcd.setCursor(0, 3);
  lcd.print(message);
}
