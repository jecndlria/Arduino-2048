int gameBoard[4][4] = 
{
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

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
    int randomLeftY = random(0, 5);

    int coinflipRight = random(0, 2);
    int randomRightX = random(2, 4);
    int randomRightY = random(0, 5);

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
}