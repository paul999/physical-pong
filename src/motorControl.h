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

#ifndef __MOTORCONTROL_H__
#define __MOTORCONTROL_H__
#include "axis.h"

void disableMotor(bool status);

long moveMotorToStart(axis moveAxis);
long moveMotorToLocation(axis moveAxis, long location, long currentLocation);

typedef struct {
    int ashort;
    int along;
    int aplayer1;
    int aplayer2;
} movement;

/**
 * Please make sure that this method does no checking at all. You are responsible for making sure start/endstops are checked
 * before calling this method!
 * You are also responsible for setting the correct direction pins for each motor. This method will not set that.
 */
void moveSeveralMotorsOneStep(movement mv);

#endif
