#include "headers/button.h"
#include "headers/eeprom.h"
#include "headers/game.h"
#include "headers/gesture.h"
#include "headers/lcd.h"
#include "headers/shiftregister_4digit7seg.h"
#include "headers/buzzer.h"

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
    
} task;

const unsigned short tasksNum = 3;
task tasks[tasksNum];

#define SAVEBUTTON 10
#define RESETBUTTON 9

enum Button_States{BUTTON_INIT, SAVE, RESET};
bool buttonHeld = false;
int buttonTick(int state)
{
    //Serial.println("value of state: ");
    //Serial.println(state);
    switch(state)
    {
        case BUTTON_INIT:
            //Serial.println("Save button: ");
            //Serial.println(digitalRead(SAVEBUTTON));
            //Serial.println("Reset button: ");
            //Serial.println(digitalRead(RESETBUTTON));
            //Serial.println("Score button: ");
            //Serial.println(digitalRead(SCOREBUTTON));
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
            else state = BUTTON_INIT;
            //Serial.println("State: BUTTON_INIT");
            break;
        case SAVE:
            state = BUTTON_INIT;
            break;
        case RESET:
            state = BUTTON_INIT;
            break;

    }
    switch(state)
    {
        case BUTTON_INIT:
            if (digitalRead(SAVEBUTTON == LOW &&
                digitalRead(RESETBUTTON) == LOW)) { buttonHeld = false; }
            else buttonHeld = true;
            //Serial.println("State: BUTTON_INIT");
            break;
        case SAVE:
            storeGameBoard();
            //Serial.println("State: SAVE");
            break;
        case RESET:
            noTone(BUZZER);
            tft.setTextColor(ST7735_BLACK);
            tft.setTextSize(1);
            tft.fillScreen(colorHelper(COLOR_2048_0));
            gameWon = false;
            score = 0;
            initializeGame();
            drawBoard();
            //Serial.println("State: RESET");
            break;
    }
    return state;
}

enum Game_States{GAME_INIT, READ_INPUT, WIN, LOSE};
bool printFlag = false;
uint8_t songIterator = 0;
int gameTick(int state)
{
    switch(state)
    {
        case GAME_INIT:
            printFlag = false;
            gameWon = false;
            noTone(BUZZER);
            state = READ_INPUT;
            break;
        case READ_INPUT:
            if (gameWon) state = WIN;
            if (checkLoss()) state = LOSE;
            break;
        case WIN:
            if (!gameWon) state = GAME_INIT;
            break;
        case LOSE:
            if (!checkLoss()) state = GAME_INIT;
            break;
    }
    switch(state)
    {
        case GAME_INIT:
            printFlag = false;
            gameWon = false;
            noTone(BUZZER);
            break;
        case READ_INPUT:
            checkMove();
            noTone(BUZZER);
            sevseg.blank();
            break;
        case WIN:
            if (!printFlag) 
            {
                songIterator = 0;
                printWin();
                sevseg.blank();
                printFlag = true;
            }
            tone(BUZZER, toneLookupTable[songIterator++]);
            if (songIterator == 12) songIterator = 0;
            break; 
        case LOSE:
            if (!printFlag)
            {
                songIterator = 11;
                printLoss();
                sevseg.blank();
                printFlag = true;
            }
            tone(BUZZER, toneLookupTable[songIterator--]);
            if (songIterator == 0) songIterator = 11;
            break;
    }

    return state;
}
enum Display_States {DISPLAY_SCORE};
int displayTick(int state)
{
    switch(state)
    {
        case DISPLAY_SCORE:
            break;   
    }
    switch(state)
    {
        case DISPLAY_SCORE:
            sevseg.setNumber(score);
            sevseg.refreshDisplay();
            break;
    }
    return state;
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
    pinMode(RESETBUTTON, INPUT);
    pinMode(SAVEBUTTON, INPUT);
    //storeGameBoard();
    loadGameBoard();
    gameBoardDebug();
    drawBoard();

    unsigned char i = 0;
    
    tasks[i].state = GAME_INIT;
    tasks[i].period = 250;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &gameTick;
    i++;
    
    tasks[i].state = BUTTON_INIT;
    tasks[i].period = 100;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &buttonTick;
    i++;

    tasks[i].state = DISPLAY_SCORE;
    tasks[i].period = 5;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &displayTick;

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