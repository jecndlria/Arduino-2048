#ifndef GAME_H
#define GAME_H
#include "lcd.h"
#include "gesture.h"
#include "shiftregister_4digit7seg.h"

//Adafruit_ST7735 tft = Adafruit_ST7735(LCD_CS, LCD_DCA0, LCD_SDA, LCD_SCK, LCD_RESET);
int gameBoard[4][4] = 
{
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int score = 0;
bool populateBoardFlag = false;

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

void resetGame()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gameBoard[i][j] = 0;
        }
    }
}

void initializeGame()
{
    int coinflipLeft = random(0, 2);
    int randomLeftX = random(0, 2);
    int randomLeftY = random(0, 2);

    int coinflipRight = random(0, 2);
    int randomRightX = random(2, 4);
    int randomRightY = random(2, 4);

    gameBoard[randomLeftX][randomLeftY] = coinflipLeft ? 4 : 2;
    gameBoard[randomRightX][randomRightY] = coinflipRight ? 4 : 2;
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
    uint8_t randomRow = random(0, 4);
    uint8_t randomColumn = random(0, 4);
    while (gameBoard[randomRow][randomColumn] != 0) 
    {
        randomRow = random(0, 4);
        randomColumn = random(0, 4);
    }
    gameBoard[randomRow][randomColumn] = 2;
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
            if (gameBoard[i][j] == gameBoard[i-1][j])
            {
                gameBoard[i-1][j] *= 2;
                clearBlock(i, j);
                score += gameBoard[i-1][j];
            }
        }
        for (int i = 1; i < 4; i++)
        {
            int k = findFurthestUp(j);
            if (k != 3)
            {
                swap(gameBoard[i][j], gameBoard[k++][j]);
                clearBlock(i, j);
            }
        }
        for (int i = 1; i < 4; i++)
        {
            if (gameBoard[i][j] == gameBoard[i-1][j])
            {
                gameBoard[i-1][j] *= 2;
                clearBlock(i, j);
                score += gameBoard[i-1][j];
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
            if (gameBoard[i][j] == gameBoard[i+1][j])
            {
                gameBoard[i+1][j] *= 2;
                clearBlock(i, j);
                score += gameBoard[i+1][j];
            }
        }
        for (int i = 2; i >= 0; i--)
        {
            int k = findFurthestDown(j);
            if (k != 0)
            {
                swap(gameBoard[i][j], gameBoard[k--][j]);
                clearBlock(i, j);
            }
        }
        for (int i = 2; i >= 0; i--)
        {
            if (gameBoard[i][j] == gameBoard[i+1][j])
            {
                gameBoard[i+1][j] *= 2;
                clearBlock(i, j);
                score += gameBoard[i+1][j];
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
            if (gameBoard[i][j] == gameBoard[i][j-1])
            {
                gameBoard[i][j-1] *= 2;
                clearBlock(i, j);
                score += gameBoard[i][j-1]; 
            }
        }
        for (int j = 1; j < 4; j++)
        {
            int k = findFurthestLeft(i);
            if (k != 3)
            {
                swap(gameBoard[i][j], gameBoard[i][k++]);
                clearBlock(i, j);
            }
        }
        for (int j = 1; j < 4; j++)
        {
            if (gameBoard[i][j] == gameBoard[i][j-1])
            {
                gameBoard[i][j-1] *= 2;
                clearBlock(i, j);
                score += gameBoard[i][j-1]; 
            }
        }
    }
}

void moveRight()
{ // j+1
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (gameBoard[i][j+1] == 0) 
            {
                swap(gameBoard[i][j], gameBoard[i][j+1]);
                clearBlock(i, j);
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
#endif