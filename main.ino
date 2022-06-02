#include "headers/button.h"
#include "headers/eeprom.h"
#include "headers/game.h"
#include "headers/gesture.h"
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

#define SAVEBUTTON 10
#define RESETBUTTON 9
#define SCOREBUTTON 8

enum Button_States{BUTTON_INIT};
int buttonTick(int state)
{
    switch(state)
    {

    }
    switch(state)
    {

    }
}

enum Game_States{GAME_INIT};
int gameTick(int state)
{
    switch(state)
    {

    }
    switch(state)
    {

    }
}

void setup()
{
    Serial.begin(9600);
    EEPROM.begin();
    randomSeed(analogRead(0));
    initializeSevenSegmentDisplay();   
    initializeScreen();
    initializeGestureSensor();
    //initializeGame();
    pinMode(SCOREBUTTON, INPUT);
    pinMode(RESETBUTTON, INPUT);
    pinMode(SAVEBUTTON, INPUT);
    //storeGameBoard();
    loadGameBoard();
    gameBoardDebug();
    drawBoard();

    unsigned char i = 0;
    tasks[i].state = GAME_INIT;
    tasks[i].period = 50;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &gameTick;
    i++;
    tasks[i].state = BUTTON_INIT;
    tasks[i].period = 50;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &buttonTick;

}
void loop()
{
    unsigned char i;
    for (i = 0; i < tasksNum; ++i) {
        if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].elapsedTime = millis(); // Last time this task was ran
        }
    }
   
    //TEST CODE
    /*
    if (digitalRead(8) == LOW) { checkMove(); sevseg.blank(); if (gameWon) { printWin(); delay(10000);}}
    else { sevseg.setNumber(score); sevseg.refreshDisplay();}
    if (digitalRead(9) == HIGH) {resetGame(); initializeGame(); drawBoard(); score = 0;}
    */
}