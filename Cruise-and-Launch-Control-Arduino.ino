/*

Todo:

Added second and third setting in settings

*/

// used to save settings.
#include <EEPROM.h>

#include "LCDPW.h"
#include "DACPW.h"
#include "TM1638PW.h"
#include "MAX7219.h"

#include "Mode.h"
#include "GlobalPW.h"

#include "LoginPW.h"
#include "CruisePW.h"
//#include "MiscPW.h"
#include "ReadPW.h"
#include "SettingsPW.h"
#include "AboutPW.h"

//#include "MiscLaunchControl.h"
#include "MiscRevTest.h"
//#include "MiscTripMaster.h"
//#include "MiscRally.h"

/* Setup:

   - LCD
   - DAC
   - Set all inputs and outputs
   - Initialize classes
   - Load layouts
*/
void setup() {
    lcd.init();
    lcd.backlight();
    MCP4725.begin(0x60);

    pinMode(ThrottleIn, INPUT);
    pinMode(SpeedIn, INPUT);
    pinMode(Relay1, OUTPUT);
    pinMode(Relay2, OUTPUT);

    Modes[0] = static_cast<Mode *>(new Read());
    Modes[1] = static_cast<Mode *>(new Cruise());
    Modes[2] = static_cast<Mode *>(new RevTest());
    //Modes[2] = static_cast<Mode *>(new Misc());
    Modes[3] = static_cast<Mode *>(new Settings());
    Modes[4] = static_cast<Mode *>(new About());
    Modes[5] = static_cast<Mode *>(new Login());

    //MiscSubModes[0] = static_cast<Mode *>(new LaunchControl());
    //MiscSubModes[1] = static_cast<Mode *>(new RevTest());
    //MiscSubModes[2] = static_cast<Mode *>(new TripMaster());

    LoadLayouts();

    SetupMatrix();
    setupMode();
}

/* Setup the current selected mode, check if logged in

*/
void setupMode() {
    lcd.clear();
    TM1638.reset();

    if (CurrentMode < 0) CurrentMode = TotalModes;
    if (CurrentMode > TotalModes) CurrentMode = 0;

    if (LoggedIn) {
        Modes[CurrentMode]->Setup();
    } else {
        Modes[5]->Setup();
    }
}

void loop() {
    handleButtons();
    RecommendedGear();

    if (LoggedIn)
        Modes[CurrentMode]->Loop();
    else
        Modes[5]->Loop();

} // every Loop function within Mode must contain a delay()

void handleButtons() {
    byte buttons = TM1638.readButtons();

    if (buttons == 0) {
        ButtonClicked = false;

        return;
    }

    // on click check
    if (!TM1638OnClick()) {
        // a button has already been pressed
        return;
    }

    // logged in check
    if (!LoggedIn) {
        Modes[5]->ButtonReceiver(buttons);
        return;
    }

    // process buttons

    switch (buttons) {
    // Previous mode 1
    case 1:
        CurrentMode -= 1;
        setupMode();
        break;

    // Next mode button 8
    case 128:
        CurrentMode += 1;
        setupMode();
        break;

    // all mode dependend buttons
    default:
        Modes[CurrentMode]->ButtonReceiver(buttons);
        break;
    }
}


/* Temporarly function until RPM is connected to the Arduino */
bool neutral = true;
void RecommendedGear() {
    if (GetSpeed() == 0 || ClutchPressed()) {
        MatrixSetGear(0);
        neutral = true;
        return;
    }

    if (neutral) {
        int gear = 1;

      for (; gear < 5; gear++) {
          if (GetRPMBasedOnSpeed(gear) < 2000) {
              break;
          }
      }

      MatrixSetGear(gear);
      neutral = false;
    }

    
}
