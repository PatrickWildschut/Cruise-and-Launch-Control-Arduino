#pragma once
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

void LCDDemo()
{
  lcd.setCursor(2, 0);
  lcd.print("This software is");

  lcd.setCursor(4, 1);
  lcd.print("licensed by:");
  
  lcd.setCursor(2, 2);
  lcd.print("Patrick Wildschut");

  lcd.setCursor(3, 3);
  lcd.print("Copyright 2023");
}

void setBanner(String message)
{
  lcd.setCursor(0, 3);
  lcd.print(message);
}
