
#define DCSBIOS_DEFAULT_SERIAL
#include "DcsBios.h"

// OPENHARRIER Project
// This code is provided AS-IS and is free and open source.
//
// Version 0.1  August 10, 2025
//
// The following code has been contribued by the OpenHarrier project.
// There is an issue in DCS-BIOS with the AV8BNA module where the
// position of the 4 canopy flood lights on module start or respawn
// of the AV8BNA, the position data for these flood lights is out of range.
// This causes DCS-BIOS on the host computer to continously spit out 100's of
// error messages per-second to the DCS-BIOS.log file on the host computer.
// The negative consequence of that is it causes a significant performance hit
// to the overall DCS-BIOS scripts running in DCS, which results in decreased
// performance of data being sent by DCS-BIOS.  
// You should put this code in only 1 Arduino sketch.  Putting this in multiple
// sketches is duplicative and not beneficial.
//
// IMPORTANT NOTES:
// * By default, this code only triggers ONCE when DCS-BIOS sends notification
//   that the aircraft module has been loaded.  DCS-BIOS does NOT receive notification
//   when AV8BNA is respawned or if you select a different player slot (from the CHOOSE SLOT menu option in DCS).
//   You can force the notification to happen by switching to spectator and THEN selecting your player slot.  
//   (Hit ESC, select "CHOOSE SLOT", then click "Back" to return to spectators, then choose coaltion and then choose your slot)
// 
// * There is a workaround for the fact that there is no notification on aircraft respawn.  You can set the
//   continuousReset flag to true, which will cause this code to retrigger every X milliseconds (recommend changing the 
//   RESET_AFTER to something like 3000 or 5000 in this case).  That way if you crash and respawn without going back to 
//   spectators, this code will fire again an clear the bad data.
//
// * When this code triggers the reset, it WILL move the canopy flood lights to the 0 position (all the way towards the main panel).
//   You could override this position to some other possition if you want.  Just change the "state" variable to something in the range
//   of 0 to 65535.
//
// * COMPASS_NS is also having an issue with bad data, but we're still investigating what's causing that.  You can try
//   uncommenting the line that resets the data for that, but since that data is always set by the sim, it will likely
//   return to a bad state immediately.   
unsigned long reset_trigger_ts = 0;
bool devices_need_reset = false;       // default to false so we don't immediate run a reset.
#define RESET_AFTER 1000               // Set this value to when you want the reset to occur, in milliseconds.  1000 = 1 second.
bool continuousReset = false;          // Set this to true if you want to continously reset the data every X milliseconds

void resetDevicesToClearBadData()
{
  if (devices_need_reset && millis() >= reset_trigger_ts)
  {
    unsigned long current_ts = millis();
    unsigned int state = 0;
    char buf[6];
    utoa(state, buf, 10);

    //send messages to force the canopy flood rings to an in-range position.
    DcsBios::tryToSendDcsBiosMessage("FLOOD_L_CAN_B", buf);
    DcsBios::tryToSendDcsBiosMessage("FLOOD_L_CAN_T", buf);
    DcsBios::tryToSendDcsBiosMessage("FLOOD_R_CAN_B", buf);
    DcsBios::tryToSendDcsBiosMessage("FLOOD_R_CAN_T", buf);

    // send message to force the whiskey compass north south heading to an in-range position.
    DcsBios::tryToSendDcsBiosMessage("COMPASS_NS", buf);

    if (!continousReset) devices_need_reset = false;
    reset_trigger_ts = current_ts + RESET_AFTER;
  }
}

void onAcftNameChange(char* newValue) {
  ////// Uncomment the commented out lines below if only want this to run when AV8BNA module first loads.
  //if (strcmp(newValue, "AV8BNA"))
  //{
    reset_trigger_ts = millis() + RESET_AFTER;  // 1-second delay
    devices_need_reset = true;
  //}
}
DcsBios::StringBuffer<24> AcftNameBuffer(0x0000, onAcftNameChange);

void setup() {
  // put your setup code here, to run once:
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
  resetDevicesToClearBadData(); // Add this to the main loop of your Arduino sketch

}



