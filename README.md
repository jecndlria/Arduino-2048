# CS-EE120B-Final-Project Spring 2022

Arduino-2048, by Joshua Candelaria

## Introduction

2048 is a single player game played on a simple 4x4 block grid where the ultimate goal is to combine blocks made of powers of two until you get a block of 2048. The game is over once a block of 2048 is reached, or if you run out of legal moves (the grid is filled, and no combinations can be made).

![Example](2048_Screenshot.png)

## Hardware Components

* Elegoo UNO R3 Microcontroller
* Joystick Module (for input)
* Buttons (for switching modes, restarting/starting)
* ST7735S 1.44" 128x128 LCD Display Screen
* Sound Buzzer 

## Basic Functionality

This is a single player game that is controlled by the joystick. The game begins with a randomly generated board, which consists of two 2-blocks in random spots of the board. All blocks will move in the direction of the joystick's movement, going as far as they can until they collide with a block. If two blocks of the same value collide, they combine into a bigger block (for example, 8 -> 8 creates a block of 16). The buzzer will make a sound whenever there is a combination, higher pitched for bigger combinations. If no combinations can be made, then one or two 2-blocks or 4-blocks spawn in empty tiles. The game is over once the player creates a block of 2048, or if no moves can be made. When the game is won, the buzzer will play a victorious song, and a somber one if the game is lost, and a new game is started. The game also has a scoring system; it is the running sum of combinations. For example, combining two 128 blocks will add 256 to your score. We can reset the game at any time by pressing the restart button.

## Complexities 

* Two 74HC595 shift registers, which will be used to control the 4-digit seven segment display that shows the current score.
* A 24C512 EEPROM, which will save the game/store the high score while the system is off.
* An Adafruit APDS9960 Gesture Sensor, which will be used as an additional mode to give input to the game.

## Demo Videos

[First Demo Video (May 20, 2022)](https://youtu.be/FuY_zUGu9N4)

