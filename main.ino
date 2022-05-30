//void initializeSevenSegmentDisplay();   // DEFINED IN shiftregister_4digit7seg.ino
//void sevenSegmentTimerTest();           // DEFINED IN shiftregister_4digit7seg.ino
//void initializeScreen();                // DEFINED IN lcd.ino
//void testScreen();                      // DEFINED IN lcd.ino
//void initializeGestureSensor();         // DEFINED IN gesture.ino
//void testGestureSensor();               // DEFINED IN gesture.ino
//void initializeGame();                  // DEFINED IN game.ino
//void gameBoardDebug();                  // DEFINED IN game.ino
#include "headers/button.h"
#include "headers/buzzer.h"
#include "headers/eeprom.h"
#include "headers/game.h"
#include "headers/gesture.h"
#include "headers/joystick.h"
#include "headers/lcd.h"
#include "headers/shiftregister_4digit7seg.h"
typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
    
} task;

const unsigned short tasksNum = 2;
task tasks[tasksNum];

void setup()
{
    Serial.begin(9600);
    randomSeed(analogRead(0));
    //initializeScreen();
    initializeSevenSegmentDisplay();   
    initializeScreen();
    //testScreen();
    initializeGestureSensor();
    initializeGame();
    gameBoardDebug();
    drawBoard();

    /*
    tasks[i].state = INIT0;
    tasks[i].period = 500;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &SM1_Tick;
    i++;
    tasks[i].state = INIT1;
    tasks[i].period = 100;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &SM2_Tick;
    */

    sevseg.setNumber(random(0, 100));
}
void loop()
{
    /*

    unsigned char i;
    for (i = 0; i < tasksNum; ++i) {
        if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].elapsedTime = millis(); // Last time this task was ran
        
        }
    }
    */
    //sevenSegmentTimerTest();
    //testGestureSensor();
    //testScreen();
}