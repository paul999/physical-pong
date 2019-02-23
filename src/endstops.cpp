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

#include "endstops.h"
#include "Arduino.h"
#include "log.h"
#include "config.h"
#include "axis.h"

int shortstartAr[7];
int shortendAr[7];
int longstartAr[7];
int longendAr[7];
int player1startAr[7];
int player1endAr[7];
int player2startAr[7]; 
int player2endAr[7];
int counter = 0;

int shortStartPin = getPin(startPin, ashort);
int shortEndPin = getPin(endPin, ashort);
int longStartPin = getPin(startPin, along);
int longEndPin = getPin(endPin, along);
int player1StartPin = getPin(startPin, aplayer1);
int player1EndPin = getPin(endPin, aplayer1);
int player2StartPin = getPin(startPin, aplayer2);
int player2EndPin = getPin(endPin, aplayer2);

void setupTimer()
{
    logging("Disabled interupts. Starting up timers");
    // Setup start registers for noise reduction
    for (int i = 0; i < 7; i++)
    {
        shortstartAr[i] = MISSED;
        shortendAr[i] = MISSED;
        longstartAr[i] = MISSED;
        longendAr[i] = MISSED;
        player1startAr[i] = MISSED;
        player1endAr[i] = MISSED;
        player2startAr[i] = MISSED;
        player2endAr[i] = MISSED;
    }

    TCCR2A = 0; // set entire TCCR2A register to 0
    TCCR2B = 0; // same for TCCR2B
    TCNT2 = 0;  //initialize counter value to 0
    // set compare match register for 8khz increments
    OCR2A = 255; // = (16*10^6) / (8000*8) - 1 (must be <256)
    // turn on CTC mode
    TCCR2A |= (1 << WGM01);
    // Set CS21 bit for 8 prescaler
    TCCR2B |= (1 << CS01) | (1 << CS00);
    // enable timer compare interrupt
    TIMSK2 |= (1 << OCIE2A);

    sei();
    logging("Done with times. Starting interupts again");
}

ISR(TIMER2_COMPA_vect)
{
    counter++;
    if (counter >= 7)
    {
        counter = 0;
    }
    shortstartAr[counter] = digitalRead(shortStartPin) == LOW ? HIT : MISSED;
    shortendAr[counter] = digitalRead(shortEndPin) == LOW ? HIT : MISSED;
    longstartAr[counter] = digitalRead(longStartPin) == LOW ? HIT : MISSED;
    longendAr[counter] = digitalRead(longEndPin) == LOW ? HIT : MISSED;
    player1startAr[counter] = digitalRead(player1StartPin) == LOW ? HIT : MISSED;
    player1endAr[counter] = digitalRead(player1EndPin) == LOW ? HIT : MISSED;
    player2startAr[counter] = digitalRead(player2StartPin) == LOW ? HIT : MISSED;
    player2endAr[counter] = digitalRead(player2EndPin) == LOW ? HIT : MISSED;
}

int calculateAverage(int result[])
{
    int hitCount = 0;

    for (int i = 0; i < 7; i++)
    {
        if (result[i] == HIT)
        {
            hitCount++;
            if (hitCount > 4)
            {
                return HIT;
            }
        }
    }
    return MISSED;
}

int readStop(axis axisName, type axisType)
{
    switch (axisName)
    {
    case ashort:
        switch (axisType)
        {
        case start:
            return calculateAverage(shortstartAr);
            break;
        case end:
            return calculateAverage(shortendAr);
            break;
        }
        break;
    case along:
        switch (axisType)
        {
        case start:
            return calculateAverage(longstartAr);
            break;
        case end:
            return calculateAverage(longendAr);
            break;
        }
        break;
    case aplayer1:
        switch (axisType)
        {
        case start:
            return calculateAverage(player1startAr);
            break;
        case end:
            return calculateAverage(player1endAr);
            break;
        }
        break;
    case aplayer2:
        switch (axisType)
        {
        case start:
            return calculateAverage(player2startAr);
            break;
        case end:
            return calculateAverage(player2endAr);
            break;
        }
        break;
    }

    return MISSED;
}