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

#include "config.h"
#include "axis.h"
#include "log.h"
#include "Arduino.h"
#include "libs/HashMap.h"

typedef struct
{
    axis axisName;
    int dirPin;
    int stepPin;
    int enabledPin;
    int startPin;
    int endPin;
} axisData;

SimpleMap<axis, axisData> *aData = new SimpleMap<axis, axisData>([](axis &a, axisData &b) -> int {
    if (a == b.axisName)
        return 0; // a and b are equal
    else if (a > b.axisName)
        return 1; // a is bigger than b
    else
        return -1; // a is smaller than b
});

axisData initData(axis axis, int dir, int step, int enable, int end, int st)
{
    axisData as;
    as.axisName = axis;
    as.dirPin = dir;
    as.stepPin = step;
    as.enabledPin = enable;
    as.endPin = end;
    as.startPin = st;
    return as;
}

void setupPins()
{
    logging("Setting up pins");

    aData->put(ashort, initData(ashort, SHORT_AXIS_DIR, SHORT_AXIS_STEP, SHORT_AXIS_EN, SHORT_AXIS_END, SHORT_AXIS_BEGIN));
    aData->put(along, initData(along, LONG_AXIS_DIR, LONG_AXIS_STEP, LONG_AXIS_EN, LONG_AXIS_END, LONG_AXIS_BEGIN));
    aData->put(aplayer1, initData(aplayer1, PLAYER1_DIR, PLAYER1_STEP, PLAYER1_EN, PLAYER1_END, PLAYER1_BEGIN));
    aData->put(aplayer2, initData(aplayer2, PLAYER2_DIR, PLAYER2_STEP, PLAYER2_EN, PLAYER2_END, PLAYER2_BEGIN));
    aData->lock();

    pinMode(START_BUTTON1, INPUT);
    pinMode(START_BUTTON2, INPUT);
    pinMode(PLAYER1_BEGIN, INPUT);
    pinMode(PLAYER1_END, INPUT);
    pinMode(PLAYER2_BEGIN, INPUT);
    pinMode(PLAYER2_END, INPUT);
    pinMode(LONG_AXIS_BEGIN, INPUT);
    pinMode(LONG_AXIS_END, INPUT);
    pinMode(SHORT_AXIS_BEGIN, INPUT);
    pinMode(SHORT_AXIS_END, INPUT);
    pinMode(PLAYER1_LEFT, INPUT);
    pinMode(PLAYER1_RIGHT, INPUT);
    pinMode(PLAYER2_LEFT, INPUT);
    pinMode(PLAYER2_RIGHT, INPUT);

    pinMode(PLAYER1_DIR, OUTPUT);
    pinMode(PLAYER1_STEP, OUTPUT);
    pinMode(PLAYER1_EN, OUTPUT);
    pinMode(PLAYER2_DIR, OUTPUT);
    pinMode(PLAYER2_STEP, OUTPUT);
    pinMode(PLAYER2_EN, OUTPUT);
    pinMode(LONG_AXIS_DIR, OUTPUT);
    pinMode(LONG_AXIS_STEP, OUTPUT);
    pinMode(LONG_AXIS_EN, OUTPUT);
    pinMode(SHORT_AXIS_DIR, OUTPUT);
    pinMode(SHORT_AXIS_STEP, OUTPUT);
    pinMode(SHORT_AXIS_EN, OUTPUT);
    pinMode(START_LED1, OUTPUT);
    pinMode(START_LED2, OUTPUT);
}

int getPin(pinType pin, axis axisName)
{
    axisData item = aData->get(axisName);

    switch (pin)
    {
    case dirPin:
        return item.dirPin;
    case stepPin:
        return item.stepPin;
    case enabledPin:
        return item.enabledPin;
    case endPin:
        return item.endPin;
    case startPin:
        return item.startPin;
    }
    return NULL;
}