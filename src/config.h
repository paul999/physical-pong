/**
 * Copyright (C) 2019 by Paul Sohier
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without l> imitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

// Enable debug mode. Will write stuff to serial for debugging
#define DEBUG false

// Enable calculation debug mode. Not suggested to enable, as it will cause
// issues with movements. Only enable it if you want to debug movement calculations
// Please note that enabling this flag will require you to press start when 
// the puck arrives at a start/stop on the long axis. 
// Make sure to read the serial output for information about the calculations and status!
#define DEBUG_CALC false

// Move to the start after a player is dead.
#define MOVE_TO_START true

// Shaft length for player/axis
// Make sure these sizes are correct, as they are used for a lot of calculations!
#define PLAYER_LENGTH 280
#define LONG_AXIS_LENGTH 480
#define SHORT_AXIS_LENGTH 280
#define STEPS_PER_MM 5

// Define the number of MM that is used for dead calculations.
#define MM_CALC 10

// Max number of games before we end
#define MAX_GAMES 10

// Begin/end stops for player bars
// X
#define PLAYER1_BEGIN   3
#define PLAYER1_END     2
// Z
#define PLAYER2_BEGIN   18
#define PLAYER2_END     19

// Begin/end stops for axis
// y
#define LONG_AXIS_BEGIN   14
#define LONG_AXIS_END     15
// AUX 2
#define SHORT_AXIS_BEGIN  16//65
#define SHORT_AXIS_END    17//66

// Motor settings for players
// X
#define PLAYER1_DIR   55//55
#define PLAYER1_STEP  54//54
#define PLAYER1_EN    38//38
// Y
#define PLAYER2_DIR   61//61
#define PLAYER2_STEP  60//60
#define PLAYER2_EN    56//56

// Motor settings for axis
// Z
#define LONG_AXIS_DIR   48
#define LONG_AXIS_STEP  46
#define LONG_AXIS_EN    62
// E
#define SHORT_AXIS_DIR  28
#define SHORT_AXIS_STEP 26
#define SHORT_AXIS_EN   24

// Buttons for going left/right with player pad
#define PLAYER1_LEFT  23
#define PLAYER1_RIGHT 25
#define PLAYER2_LEFT  27
#define PLAYER2_RIGHT 29    	

// Start button + start LED in button (Will light on when no game is active)
// Aux 1
#define START_BUTTON1  57
#define START_BUTTON2  58
// Heatbed connector
#define START_LED1     9
#define START_LED2     10

// Size of the player (Only width) blocks and actual block. Please keep in mind that the block should be square.
// Only change if you changed the 3D printed parts!
#define PLAYER_SIZE 45
#define BLOCK_SIZE 25

// Do not change after this line
#define PLAYING 1
#define WAITING_FOR_START 2
#define RESTARTING 3

#endif