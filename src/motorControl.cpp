/**
   Copyright (C) 2019 by Paul Sohier

   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without l> imitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "Arduino.h"
#include "log.h"
#include "config.h"
#include "endstops.h"
#include "axis.h"
#include "motorControl.h"

int moveSequence(axis moveAxis)
{
  digitalWrite(getPin(stepPin, moveAxis), HIGH);
  delayMicroseconds(300);
  digitalWrite(getPin(stepPin, moveAxis), LOW);
  delayMicroseconds(300);

  if (readStop(moveAxis, end))
  {
    // Huh, we did hit a end?
    logging("We did hit a endstop?");
    return 1;
  }
  if (readStop(moveAxis, start))
  {
    logging("We did hit a start");
    return -1;
  }

  return 0;
}
void disableMotor(bool status)
{
  int st = status ? HIGH : LOW;
  digitalWrite(PLAYER1_EN, st);
  digitalWrite(PLAYER2_EN, st);
  digitalWrite(LONG_AXIS_EN, st);
  digitalWrite(SHORT_AXIS_EN, st);
}

long moveMotorToStart(axis moveAxis)
{
  if (readStop(moveAxis, start) == HIT)
  {
    // Motor is already in start Position.
    logging("Motor is at start");
    return 0;
  }
  digitalWrite(getPin(dirPin, moveAxis), HIGH);
  boolean atEnd = false;
  if (readStop(moveAxis, end) == HIT)
  {
    logging("Motor is at end");
    atEnd = true;
  }
  logging("Starting loop");

  long i = 0;

  while (true)
  {
    i++;
    if (i > 100000l)
    {
      // 200 steps per mm, max length is ~500mm. So 200 * 500: 100000 should always result in the start!
      logging("Unable to get to start");
      disableMotor(true);
      return -1;
    }
    if (!atEnd && readStop(moveAxis, end) == HIT)
    {
      logging("Looks like we hit a endstop while going to home. Wrongly configured?");
      logging("We will return now, but we are not in a save position!");
      disableMotor(true);
      return -1;
    }
    if (atEnd && readStop(moveAxis, end) == MISSED)
    {
      atEnd = false;
    }
    if (readStop(moveAxis, start) == HIT)
    {
      logging("Arrived at start :D");
      return 0;
    }
    digitalWrite(getPin(stepPin, moveAxis), HIGH);
    delayMicroseconds(200);
    digitalWrite(getPin(stepPin, moveAxis), LOW);
    delayMicroseconds(200);
  }
}

long moveMotorToLocation(axis moveAxis, long location, long currentLocation)
{
  if (location == currentLocation)
  {
    return currentLocation;
  }
  if (location <= currentLocation)
  {
    digitalWrite(getPin(dirPin, moveAxis), HIGH);
  }
  else
  {
    digitalWrite(getPin(dirPin, moveAxis), LOW);
  }

  long newCurrent = currentLocation;

  long diff = location - currentLocation;
  Serial.println("diff:");
  Serial.println(diff);

  if (diff < 0)
  {
    diff *= -1;
    for (long i = 0; i < diff; i++)
    {
      newCurrent += 1;
      int result = moveSequence(moveAxis);
      switch (result)
      {
      case -1:
        //return 0; // Start
        break;

      case 1:
        return newCurrent; // TODO: Check correct value.
        break;
      case 0:
        // Do nothing.
        break;
      }
    }
  }
  else
  {
    for (long i = 0; i < diff; i++)
    {
      newCurrent += 1;
      int result = moveSequence(moveAxis);

      switch (result)
      {
      case -1:
        //return 0; // Start
        break;

      case 1:
        return newCurrent; // TODO: Check correct value.
        break;
      case 0:
        // Do nothing.
        break;
      }
    }
  }

  return newCurrent;
}

void moveSeveralMotorsOneStep(movement mv)
{
  digitalWrite(getPin(stepPin, along), mv.along);
  digitalWrite(getPin(stepPin, ashort), mv.ashort);
  digitalWrite(getPin(stepPin, aplayer1), mv.aplayer1);
  digitalWrite(getPin(stepPin, aplayer2), mv.aplayer2);
  delayMicroseconds(750);
  digitalWrite(getPin(stepPin, along), LOW);
  digitalWrite(getPin(stepPin, ashort), LOW);
  digitalWrite(getPin(stepPin, aplayer1), LOW);
  digitalWrite(getPin(stepPin, aplayer2), LOW);
  delayMicroseconds(700);
}