#pragma once
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

void LCDDemo() {
    lcd.setCursor(1, 0);
    lcd.print("Ford Fiesta MK6 S");

    lcd.setCursor(0, 1);
    lcd.print("--------------------");

    lcd.setCursor(6, 2);
    lcd.print("Owned by");

    lcd.setCursor(1, 3);
    lcd.print("Patrick Wildschut");
}

void LCDreset() {
    for (int i = 0; i < 3; i++) {
        lcd.setCursor(0, i);
        lcd.print("                    ");
    }
}

void LCDreset(int y) {
    lcd.setCursor(0, y);
    lcd.print("                    ");
}

void setBanner(String message) {
    LCDreset();
    lcd.setCursor(0, 3);
    lcd.print(message);
}
