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

int moveSequence(int motor, int end, int start)
{
  digitalWrite(motor, HIGH);
  delayMicroseconds(500);
  digitalWrite(motor, LOW);
  delayMicroseconds(500);

  if (digitalRead(end) == LOW)
  {
    // Huh, we did hit a end?
    logging("We did hit a endstop?");
    return 1;
  }
  if (digitalRead(start) == LOW)
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

long moveMotorToStart(int motor, int directionPin, int startPin, int endPin)
{
  if (digitalRead(startPin) == LOW)
  {
    // Motor is already in start Position.
    logging("Motor is at start");
    return 0;
  }
  digitalWrite(directionPin, HIGH);
  boolean atEnd = false;
  if (digitalRead(endPin) == LOW)
  {
    logging("Motor is at end");
    atEnd = true;
  }

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
    if (!atEnd && digitalRead(endPin) == LOW)
    {
      logging("Looks like we hit a endstop while going to home. Wrongly configured?");
      logging("We will return now, but we are not in a save position!");
      disableMotor(true);
      return -1;
    }
    if (atEnd && digitalRead(endPin) == HIGH)
    {
      atEnd = false;
    }
    if (digitalRead(startPin) == LOW)
    {
      logging("Arrived at start :D");
      return 0;
    }
    digitalWrite(motor, HIGH);
    delayMicroseconds(500);
    digitalWrite(motor, LOW);
    delayMicroseconds(500);
  }
}

long moveMotorToLocation(int motor, int directionPin, int startPin, int endPin, long location, long currentLocation)
{
  if (location == currentLocation)
  {
    return currentLocation;
  }
  if (location <= currentLocation)
  {
    digitalWrite(directionPin, HIGH);
  }
  else
  {
    digitalWrite(directionPin, LOW);
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
      int result = moveSequence(motor, endPin, startPin);
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
      int result = moveSequence(motor, endPin, startPin);

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

void moveSeveralMotorsOneStep(int number, int motor[])
{
  for (int i = 0; i < number ; i++)
  {
    digitalWrite(motor[i], HIGH);
  }
  delayMicroseconds(750);
  for (int i = 0; i < number; i++)
  {
    digitalWrite(motor[i], LOW);
  }
  delayMicroseconds(700);
}