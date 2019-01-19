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

// Enable debug mode. Will write stuff to serial for debugging
#define DEBUG true

// Begin/end stops for player bars
#define PLAYER1_BEGIN   1
#define PLAYER1_END     1
#define PLAYER2_BEGIN   1
#define PLAYER2_END     1

// Begin/end stops for axis
#define LONG_AXIS_BEGIN   1
#define LONG_AXIS_END     1
#define SHORT_AXIS_BEGIN  1
#define SHORT_AXIS_END    1

// Motor settings for players
#define PLAYER1_DIR   1
#define PLAYER1_STEP  1
#define PLAYER2_DIR   1
#define PLAYER2_STEP  1

// Motor settings for axis
#define LONG_AXIS_DIR   1
#define LONG_AXIS_STEP  1
#define SHORT_AXIS_DIR  1
#define SHORT_AXIS_STEP 1

// Buttons for going left/right with player pad
#define PLAYER1_LEFT  1
#define PLAYER1_RIGHT 1
#define PLAYER2_LEFT  1
#define PLAYER2_RIGHT 1

// Start button + start LED in button (Will light on when no game is active)
#define START_BUTTON  1
#define START_LED     1

// Do not change after this line
#define PLAYING 1
#define WAITING_FOR_START 2
int mode = WAITING_FOR_START;
