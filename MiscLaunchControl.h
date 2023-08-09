// Launch Control functions from within Misc

bool isPressedBefore = false;
bool started = false;

// Button 7 press
void LaunchControlChoose()
{
  started = false;
  isPressedBefore = false;
}

void rev()
{
  // Clutch is pressed, rev until release, then go into 'started'
  lcd.setCursor(1, 0);
  lcd.print("Revving to 4k RPM");

  lcd.setCursor(1, 1);
  lcd.print("Release clutch to");

  lcd.setCursor(7, 2);
  lcd.print("LAUNCH");

  flashLEDs();
  setRelays(true);
  SetVoltage(1.3);
  isPressedBefore = true;  
}

void launched()
{
  // LAUNCH
  LCDreset(0);
  LCDreset(2);
  setRelays(false);
  SetVoltage(idleVoltage);

  lcd.setCursor(0, 1);
  lcd.print("    LAUNCHED!!!   ");

  setLEDs(true);
  started = true;
}

void launchIdle()
{
  lcd.setCursor(3, 0);
  lcd.print("Depress clutch");

  lcd.setCursor(6, 1);
  lcd.print("to start");

  setRelays(false);
  walkingLEDs();
}

void LaunchControl()
{
  if(ClutchPressed() && !started)
  {
    rev();
  } 
  else
  {
    if(isPressedBefore)
    {
      launched();
    } else
    {
      launchIdle();
    }
  }

  TM1638Banner("E.    .3");

  delay(100);
}