#pragma once

void setupCruise()
{
  pinMode(Throttle3, OUTPUT);
  pinMode(Throttle6, OUTPUT);

  setBanner("Current mode: Cruise");
}

void Cruise()
{
  lcd.setCursor(0, 0);
  lcd.print("To do...");
}
