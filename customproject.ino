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

const unsigned short tasksNum = 1;
task tasks[tasksNum];

#define SAVEBUTTON 10
#define RESETBUTTON 9
#define SCOREBUTTON 8

enum Button_States{BUTTON_INIT, SAVE, RESET, SCORE};
bool buttonHeld = false;
int buttonTick(int state)
{
    switch(state)
    {
        default:
            state = BUTTON_INIT;
        case BUTTON_INIT:
            Serial.println("Save button: ");
            Serial.println(digitalRead(SAVEBUTTON));
            Serial.println("Reset button: ");
            Serial.println(digitalRead(RESETBUTTON));
            Serial.println("Score button: ");
            Serial.println(digitalRead(SCOREBUTTON));
            if (digitalRead(SAVEBUTTON) == HIGH && !buttonHeld) 
            {
                buttonHeld = true;
                state = SAVE;
            }
            else if (digitalRead(RESETBUTTON) == HIGH && !buttonHeld)
            {
                buttonHeld = true;
                state = RESET;
            }
            else if (digitalRead(SCOREBUTTON) == HIGH && !buttonHeld)
            {
                buttonHeld = true;
                state = SCORE;
            }
            else state = BUTTON_INIT;
            Serial.println("State: BUTTON_INIT");
            break;
        case SAVE:
            state = BUTTON_INIT;
            break;
        case RESET:
            state = BUTTON_INIT;
            break;
        case SCORE:
            if (digitalRead(SCOREBUTTON) == LOW) 
            { 
                state = BUTTON_INIT;
                sevseg.blank(); 
            }
            break;
    }
    switch(state)
    {
        case BUTTON_INIT:
            if (digitalRead(SAVEBUTTON == LOW &&
                digitalRead(RESETBUTTON) == LOW &&
                digitalRead(SCOREBUTTON) == LOW)) { buttonHeld = false; }
            Serial.println("State: BUTTON_INIT");
            break;
        case SAVE:
            storeGameBoard();
            Serial.println("State: SAVE");
            break;
        case RESET:
            initializeGame();
            drawBoard();
            Serial.println("State: RESET");
            break;
        case SCORE:
            sevseg.setNumber(score);
            sevseg.refreshDisplay();
            Serial.println("State: SCORE");
            break;
    }
}

enum Game_States{GAME_INIT};
int gameTick(int state)
{
    switch(state)
    {
        case GAME_INIT:
        break;
    }
    switch(state)
    {
        case GAME_INIT:
        break;
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
    initializeGame();
    pinMode(SCOREBUTTON, INPUT);
    pinMode(RESETBUTTON, INPUT);
    pinMode(SAVEBUTTON, INPUT);
    //storeGameBoard();
    //loadGameBoard();
    gameBoardDebug();
    drawBoard();

    unsigned char i = 0;
    /*
    tasks[i].state = GAME_INIT;
    tasks[i].period = 250;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &gameTick;
    i++;
    */
    tasks[i].state = BUTTON_INIT;
    tasks[i].period = 250;
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