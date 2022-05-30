#ifndef SHIFT_H
#define SHIFT_H
#include "SevSegShift.h"

#define SHIFT_PIN_DS   13 /* Data input PIN */
#define SHIFT_PIN_STCP 12 /* Shift Register Storage PIN */
#define SHIFT_PIN_SHCP 11 /* Shift Register Shift PIN */
int sevenSegmentTimerTestValue = 0; 
int sevenSegmentTimerDelayTest = 0;

SevSegShift sevseg(SHIFT_PIN_DS, SHIFT_PIN_SHCP, SHIFT_PIN_STCP); //Instantiate a seven segment controller object (with Shift Register functionality)


/* 
IMPORTANT FUNCTION! PUT THIS IN SETUP! 
INITIALIZES THE DISPLAY SO IT WORKS
*/
void initializeSevenSegmentDisplay()
{
  byte numDigits = 4;
  byte digitPins[] = {8+2, 8+5, 8+6, 2}; // of ShiftRegister(s) | 8+x (2nd Register)
  byte segmentPins[] = {8+3, 8+7, 4, 6, 7, 8+4, 3,  5}; // of Shiftregister(s) | 8+x (2nd Register)
  /* configuration without ShiftRegister - Direct arduino connection
  byte digitPins[] = {2, 3, 4, 5}; // PINs of Arduino
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13}; // PINs of Arduino
  */
  bool resistorsOnSegments = true; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  sevenSegmentTimerTestValue = 0;
  sevenSegmentTimerDelayTest = 0;
}

void sevenSegmentTimerTest()
{

  sevseg.refreshDisplay();

  if (sevenSegmentTimerDelayTest == 10000) {
    sevseg.setNumber(sevenSegmentTimerTestValue++);
    sevenSegmentTimerDelayTest = 0;
  }
  else {
    sevenSegmentTimerDelayTest++;
  }
}
#endif