#pragma once

void setupLaunch()
{
  pinMode(Throttle3, OUTPUT);
  pinMode(Throttle6, OUTPUT);

  setBanner("Current mode: Launch");
}

void Launch()
{
  lcd.setCursor(0, 0);
  lcd.print("To do...");
}
