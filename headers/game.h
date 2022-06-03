#ifndef GAME_H
#define GAME_H
#include "lcd.h"
#include "gesture.h"
#include "shiftregister_4digit7seg.h"
#include "buzzer.h"
int gameBoard[4][4] = 
{ 
    //LOSS DEMO
    /*
    {2, 4, 8, 0},
    {32, 64, 128, 256},
    {512, 256, 512, 2},
    {1024, 4, 8, 16}
    */

   
   {0, 0, 0, 0},
   {0, 0, 0, 0},
   {0, 0, 0, 0},
   {0, 0, 0, 0}
    
   // WIN DEMO
   /*
   {1024, 0, 0, 1024},
   {0, 0, 0, 0},
   {0, 0, 0, 0},
   {1024, 0, 0, 1024}
  */
};

int score = 0;
bool populateBoardFlag = false;
bool gameWon = false;

void swap(int &a, int &b)
{
    int c = a;
    a = b;
    b = c;
}

void clearBlock(int i, int j)
{
    gameBoard[i][j] = 0;
    tft.fillRect(32 * j, 32 * i, 32, 32, colorLookupTable[0]);
}

// "Why do you need this?"
// 1. The version of log base 2 in the STL uses float values. I don't want to introduce that inaccuracy.
// 2. This is faster. I COULD make it faster using binary search but that's too much work.
// 3. The main reason: it allows me to index MUCH easier within my color
// lookup table just by passing in the value of whatever block we're looking at.
// Floats don't let me do that.
int logBase2(int x)
{
    if (x == 2) return 1;
    else if (x == 4) return 2;
    else if (x == 8) return 3;
    else if (x == 16) return 4;
    else if (x == 32) return 5;
    else if (x == 64) return 6;
    else if (x == 128) return 7;
    else if (x == 256) return 8;
    else if (x == 512) return 9;
    else if (x == 1024) return 10;
    else if (x == 2048) return 11;
    else return 0;
}


bool isFull()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (gameBoard[i][j] == 0) return false;
        }
    }
    return true;
}
bool checkLoss()
{
    if (!isFull()) return false;
    for (int i = 1; i < 3; i++)
    {
        for (int j = 1; j < 3; j++)
        {
            if (gameBoard[i][j] == gameBoard[i-1][j] ||
                gameBoard[i][j] == gameBoard[i+1][j] ||
                gameBoard[i][j] == gameBoard[i][j-1] ||
                gameBoard[i][j] == gameBoard[i][j+1]) 
            return false;
        }
    }
    if (gameBoard[0][0] == gameBoard[0][1]) return false;
    if (gameBoard[0][0] == gameBoard[1][0]) return false;
    if (gameBoard[0][3] == gameBoard[0][2]) return false;
    if (gameBoard[0][3] == gameBoard[1][3]) return false;
    if (gameBoard[3][0] == gameBoard[2][0]) return false;
    if (gameBoard[3][0] == gameBoard[3][1]) return false;
    if (gameBoard[3][3] == gameBoard[3][2]) return false;
    if (gameBoard[3][3] == gameBoard[2][3]) return false;
    return true;
}
void resetGame()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gameBoard[i][j] = 0;
        }
    }
    score = 0;
}

void initializeGame()
{
    resetGame();
    int coinflipLeft = random(0, 2);
    int randomLeftX = random(0, 2);
    int randomLeftY = random(0, 2);

    int coinflipRight = random(0, 2);
    int randomRightX = random(2, 4);
    int randomRightY = random(2, 4);

    gameBoard[randomLeftX][randomLeftY] = coinflipLeft ? 4 : 2;
    gameBoard[randomRightX][randomRightY] = coinflipRight ? 4 : 2;
    score = 0;
}

void gameBoardDebug()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Serial.print(gameBoard[i][j]);
            Serial.print(' ');
        }
        Serial.print('\n');
    }
    Serial.print('\n');
}

void drawBoard()
{
    tft.setTextColor(ST7735_BLACK);
    tft.setTextSize(1);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            tft.fillRect(32 * j, 32 * i, 32, 32, colorLookupTable[logBase2(gameBoard[i][j])]);
            tft.setCursor(4 + 32*j, 16+32*i);
            if (gameBoard[i][j] != 0) tft.print(gameBoard[i][j]);
        }
    }
}

void populateBoard()
{
    bool boardFull = isFull();
    if (!populateBoardFlag || boardFull) return;
    populateBoardFlag = false;
    uint8_t randomRow = random(0, 4);
    uint8_t randomColumn = random(0, 4);
    while (!boardFull && gameBoard[randomRow][randomColumn] != 0) 
    {
        randomRow = random(0, 4);
        randomColumn = random(0, 4);
    }
    uint8_t oneInTen = random(0, 10);
    if (!boardFull) gameBoard[randomRow][randomColumn] = oneInTen ? 2 : 4;
}

int findFurthestUp(int j)
{
    for (int i = 0; i < 4; i++)
    {
        if (gameBoard[i][j] == 0) return i;
    }
    return 3;
}
void moveUp()
{
    for (int j = 3; j >= 0; j--)
    {
        for (int i = 1; i < 4; i++)
        {
            if (gameBoard[i][j] != 0 && gameBoard[i][j] == gameBoard[i-1][j])
            {
                gameBoard[i-1][j] *= 2;
                tone(BUZZER, toneLookupTable[logBase2(gameBoard[i][j])]);
                clearBlock(i, j);
                score += gameBoard[i-1][j];
                if (gameBoard[i-1][j] == 2048) gameWon = true;
            }
        }
        for (int i = 1; i < 4; i++)
        {
            int k = findFurthestUp(j);
            if (k != 3)
            {
                swap(gameBoard[i][j], gameBoard[k++][j]);
                clearBlock(i, j);
                populateBoardFlag = true;
            }
        }
        for (int i = 1; i < 4; i++)
        {
            if (gameBoard[i][j] != 0 && gameBoard[i][j] == gameBoard[i-1][j])
            {
                gameBoard[i-1][j] *= 2;
                tone(BUZZER, toneLookupTable[logBase2(gameBoard[i][j])]);
                clearBlock(i, j);
                score += gameBoard[i-1][j];
                if (gameBoard[i-1][j] == 2048) gameWon = true;
                populateBoardFlag = true;
            }
        }
    }
}

int findFurthestDown(int j)
{
    for (int i = 3; i >= 0; i--)
    {
        if (gameBoard[i][j] == 0) return i;
    }
    return 0;
}

void moveDown()
{
    for (int j = 0; j < 4; j++)
    {
        for (int i = 2; i >= 0; i--)
        {
            if (gameBoard[i][j] != 0 && gameBoard[i][j] == gameBoard[i+1][j])
            {
                gameBoard[i+1][j] *= 2;
                tone(BUZZER, toneLookupTable[logBase2(gameBoard[i][j])]);
                clearBlock(i, j);
                score += gameBoard[i+1][j];
                if (gameBoard[i+1][j] == 2048) gameWon = true;
            }
        }
        for (int i = 2; i >= 0; i--)
        {
            int k = findFurthestDown(j);
            if (k != 0)
            {
                swap(gameBoard[i][j], gameBoard[k--][j]);
                clearBlock(i, j);
                populateBoardFlag = true;
            }
        }
        for (int i = 2; i >= 0; i--)
        {
            if (gameBoard[i][j] != 0 && gameBoard[i][j] == gameBoard[i+1][j])
            {
                gameBoard[i+1][j] *= 2;
                tone(BUZZER, toneLookupTable[logBase2(gameBoard[i][j])]);
                clearBlock(i, j);
                score += gameBoard[i+1][j];
                if (gameBoard[i+1][j] == 2048) gameWon = true;
                populateBoardFlag = true;
            }
        }
    }
}

int findFurthestLeft(int i)
{
    for (int j = 0; j < 4; j++)
    {
        if (gameBoard[i][j] == 0) return j;
    }
    return 3;
}

void moveLeft()
{ // j-1
    for (int i = 0; i < 4; i++)
    {
        for (int j = 1; j < 4; j++)
        {
            if (gameBoard[i][j] != 0 && gameBoard[i][j] == gameBoard[i][j-1])
            {
                gameBoard[i][j-1] *= 2;
                tone(BUZZER, toneLookupTable[logBase2(gameBoard[i][j])]);
                clearBlock(i, j);
                score += gameBoard[i][j-1]; 
                if (gameBoard[i][j-1] == 2048) gameWon = true;
            }
        }
        for (int j = 1; j < 4; j++)
        {
            int k = findFurthestLeft(i);
            if (k != 3)
            {
                swap(gameBoard[i][j], gameBoard[i][k++]);
                clearBlock(i, j);
                populateBoardFlag = true;
            }
        }
        for (int j = 1; j < 4; j++)
        {
            if (gameBoard[i][j] != 0 && gameBoard[i][j] == gameBoard[i][j-1])
            {
                gameBoard[i][j-1] *= 2;
                tone(BUZZER, toneLookupTable[logBase2(gameBoard[i][j])]);
                clearBlock(i, j);
                score += gameBoard[i][j-1]; 
                if (gameBoard[i][j-1] == 2048) gameWon = true;
                populateBoardFlag = true;
            }
        }
    }
}

int findFurthestRight(int i)
{
    for (int j = 3; j >= 0; j--)
    {
        if (gameBoard[i][j] == 0) return j;
    }
    return 0;
}

void moveRight()
{ // j+1
    for (int i = 0; i < 4; i++)
    {
        for (int j = 2; j >= 0; j--)
        {
            if (gameBoard[i][j] != 0 && gameBoard[i][j] == gameBoard[i][j+1])
            {
                gameBoard[i][j+1] *= 2;
                tone(BUZZER, toneLookupTable[logBase2(gameBoard[i][j])]);
                clearBlock(i, j);
                score += gameBoard[i][j+1];
                if (gameBoard[i][j+1] == 2048) gameWon = true;
            }
        }
        for (int j = 2; j >= 0; j--)
        {
            int k = findFurthestRight(i);
            if (k != 0)
            {
                swap(gameBoard[i][j], gameBoard[i][k--]);
                clearBlock(i, j);
                populateBoardFlag = true;
            }
        }
        for (int j = 2; j >= 0; j--)
        {
            if (gameBoard[i][j] != 0 && gameBoard[i][j] == gameBoard[i][j+1])
            {
                gameBoard[i][j+1] *= 2;
                tone(BUZZER, toneLookupTable[logBase2(gameBoard[i][j])]);
                clearBlock(i, j);
                score += gameBoard[i][j+1];
                if (gameBoard[i][j+1] == 2048) gameWon = true;
                populateBoardFlag = true;
            }
        }
    }

}

void checkMove()
{
    uint8_t gesture = apds.readGesture();
    if (gesture == APDS9960_UP) {Serial.println("UP"); moveUp(); populateBoard(); drawBoard(); /*gameBoardDebug();*/}
    if (gesture == APDS9960_DOWN) {Serial.println("DOWN"); moveDown(); populateBoard(); drawBoard();  /*gameBoardDebug();*/}
    if (gesture == APDS9960_LEFT) {Serial.println("LEFT"); moveLeft(); populateBoard(); drawBoard(); /*gameBoardDebug();*/ }
    if (gesture == APDS9960_RIGHT) {Serial.println("RIGHT"); moveRight(); populateBoard(); drawBoard(); /*gameBoardDebug();*/}
}

void printWin()
{
    tft.fillScreen(ST7735_GREEN);
    tft.setCursor(32, 32);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(4);
    tft.println("You");
    tft.setCursor(28, 64);
    tft.println("Win!");
}
void printLoss()
{
    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(32, 32);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(4);
    tft.println("You");
    tft.setCursor(24,64);
    tft.println("Lose");
}
void songWin()
{
    for (uint8_t i = 0; i < 12; i++)
    {
        tone(BUZZER, toneLookupTable[i]);
        if (i == 11) i = 0;
    }
}
void songLoss()
{
    for (uint8_t i = 11; i > 0; i--)
    {
        tone(BUZZER, toneLookupTable[i]);
        if (i == 1) i = 11;
    }
}
#endif