#include <LiquidCrystal_I2C.h>

#include "GlobalPW.h"
#include "TM1638PW.h"
#include "CruisePW.h"
#include "LaunchPW.h"
#include "ReadPW.h"

// 0: Read. 1: Cruise control, 2: Launch control
int Mode = 0;

void setup() {
  // put your setup code here, to run once:
  lcd.init(); 
  lcd.backlight();
  TM1638.reset();

  switch (Mode)
  {
    case 0:
      setupRead();
      break;
    case 1:
      setupCruise();
      break;
    case 2:
      setupLaunch();
  }
}

void loop() {
  switch (Mode)
  {
    case 0:
      Read();
      break;
    case 1:
      Cruise();
      break;
    case 2:
      Launch();
      break;
  }
  walkingLEDs();
  delay(100);
}
