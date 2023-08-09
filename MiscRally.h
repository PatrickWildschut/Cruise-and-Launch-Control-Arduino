// Rally / Track mode. 

/*
This mode includes a timer, current speed and distance driven.

The timer will start counting when you start driving.
You end the timer with a button press, after that, the program will automatically calculate 
the average speed and will show you an end summary of the rally course or track driven.

Todo:
fix fixing
add a button for lap times, reset timer and save old timer and distance driven.
*/

bool ready = false;
bool rStarted = false;

unsigned long startTime = 0;
unsigned long timeElapsed = 0;

float rallyDistance = 0;

// Button 7 press
void RallyChoose()
{
  // end rally
  // reset

  // TEST
  rallyDistance = 0;
  startTime = millis();
}

String getMinutes()
{
  int timeSeconds = timeElapsed / 1000;

  short minutes = timeSeconds / 60;

  if(minutes < 10) return " " + String(minutes);

  return String(minutes);
}

String getSeconds()
{
  int timeSeconds = timeElapsed / 1000;

  short seconds = timeSeconds % 60;

  if(seconds < 10) return "0" + String(seconds);

  return String(seconds);
}

String Timer()
{
  timeElapsed = millis() - startTime;
  

  String timeStr = String(timeElapsed);

  if(timeElapsed < 999)
    return " 0:00:" + String(timeElapsed);
  
  if(timeElapsed < 9999)
    return " 0:" + String(timeStr.charAt(0)) + ":"
        + timeStr.substring(1);
  
  if(timeElapsed < 59999)
    return " 0:" + String(timeStr.charAt(0)) + String(timeStr.charAt(1)) + ":"
        + timeStr.substring(2);

  return getMinutes() + ":" + getSeconds() + ":" + timeStr.substring(timeStr.length() - 3);
}

void rallyStarted()
{
  // time elapsed TL
  lcd.setCursor(0, 0);
  lcd.print("Time Elapsed");

  lcd.setCursor(1, 1);
  lcd.print(Timer());

  // current speed TR
  lcd.setCursor(14, 0);
  lcd.print("Speed");

  lcd.setCursor(15, 1);
  lcd.print(String(int(GetSpeed())));

  // throttle position BL
  lcd.setCursor(0, 2);
  lcd.print("Throttle");

  short percentage = GetThrottlePercentage();
  lcd.setCursor(3, 3);
  lcd.print(PercentageToText(percentage));

  // distance driven BR
  lcd.setCursor(11, 2);
  lcd.print("Distance");

  rallyDistance += GetDistance();

  lcd.setCursor(14, 3);
  lcd.print(String(int(rallyDistance)));
}

void rallyReady()
{
  lcd.setCursor(3, 1);
  lcd.print("Drive to start.");

  lcd.setCursor(5, 3);
  lcd.print("  Ready.  ");
}

void rallyNotReady()
{
  lcd.setCursor(3, 1);
  lcd.print("Stop to start.");

  lcd.setCursor(5, 3);
  lcd.print("Not Ready.");
}

void rallyIdle()
{

  lcd.setCursor(1, 0);
  lcd.print("Rally / Track Mode");

  lcd.setCursor(6, 2);
  lcd.print("Status:");

  ready ? rallyReady() : rallyNotReady();  

  if(!ready)
  {
    // wait until the car is stationary, then we're ready for rally
    ready = GetSpeed() == 0;

    return;
  }

  // we are ready, wait until the speed is not 0 anymore. 
  if(GetSpeed() != 0)
  {
    // start rally
    rStarted = true;
    startTime = millis();
  }
}

void Rally()
{
  if(rStarted)
  {
    rallyStarted();
  } 
  else
  {
    rallyIdle();
  }

  TM1638Banner("E.    .3");

  delay(30);
}