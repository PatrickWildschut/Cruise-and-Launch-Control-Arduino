#pragma once
#include <LedControl.h>

const byte DIN = 7;
const byte CS = 4;
const byte CLK = 2;
LedControl matrix(DIN, CLK, CS, 0);

// Gears
const byte One[] = {B00000011, B00000011, B11111111, B11111111, B11111111, B11111111, B01100011, B00100011};
const byte Two[] = {B11111011, B11111011, B11011011, B11011011, B11011011, B11011011, B11011111, B11011111};
const byte Three[] = {B11111111, B11111111, B11011011, B11011011, B11011011, B11011011, B11011011, B11011011};
const byte Four[] = {B00001100, B00001100, B11111111, B11111111, B11001100, B01101100, B00111100, B00011100};
const byte Five[] = {B11011111, B11011111, B11011011, B11011011, B11011011, B11011011, B11111011, B11111011};
const byte Neutral[] = {B11111111, B11111111, B00001110, B00011100, B00111000, B01110000, B11111111, B11111111};

void SetupMatrix() {
    matrix.shutdown(0, false);
    matrix.setIntensity(0, 3);
    matrix.clearDisplay(0);
}

void matrix_setRow(byte input[]) {
    for (int i = 0; i < 8; i++) {
        matrix.setRow(0, i, input[i]);
    }
}

void MatrixSetGear(int gear) {
    switch (gear) {
    case 1:
        matrix_setRow(One);
        break;
    case 2:
        matrix_setRow(Two);
        break;
    case 3:
        matrix_setRow(Three);
        break;
    case 4:
        matrix_setRow(Four);
        break;
    case 5:
        matrix_setRow(Five);
        break;
    default:
        matrix_setRow(Neutral);
        break;
    }
}
