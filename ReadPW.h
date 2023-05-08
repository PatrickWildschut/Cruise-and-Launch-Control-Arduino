#pragma once

float value3 = 0;
float value6 = 0;

void setupRead()
{
  pinMode(Throttle3, INPUT);
  pinMode(Throttle6, INPUT);

  setBanner("Current mode: Read");
}

void Read()
{
  // Read pins
  value3 = analogRead(Throttle3);
  value3 *= 5.0 / 1023.0;
  
  value6 = analogRead(Throttle6);
  value6 *= 5.0 / 1023.0;

  // Display to I2C
  lcd.setCursor(0, 0);
  lcd.print("3: " + String(value3) + " volt");

  lcd.setCursor(0, 1);
  lcd.print("6: " + String(value6) + " volt");

  // TEST
  TM1638.displayText(String(value3) + String(value6));
}
