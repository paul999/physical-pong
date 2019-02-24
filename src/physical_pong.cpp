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

#include "Arduino.h"
#include "config.h"
#include "motorControl.h"
#include "endstops.h"
#include "axis.h"
#include "log.h"

long player1_location = 0;
long player2_location = 0;
long short_axis_location = 0;
long long_axis_location = 0;
int mode = WAITING_FOR_START;
int current_long_direction = 0;
int current_short_direction = 0;

int short_last_change = 0;
int long_last_change = 0;
int steps_min_needed = 20; //ceil(STEPS_PER_MM * 3);

void setup()
{
  cli();
  Serial.begin(9600);
  logging("Starting up");
  setupPins();
  setupTimer();

  randomSeed(analogRead(0));

  logging("Startup done");

  disableMotor(false);

  logging("Going to start positions");

  player1_location = moveMotorToStart(aplayer1);
  player2_location = moveMotorToStart(aplayer2);

  logging("Moving axis");
  long_axis_location = moveMotorToStart(along);
  short_axis_location = moveMotorToStart(ashort);

  disableMotor(true);

  logging("Startup done. Now time to startup the loop");
}

void loop()
{
  if (mode == PLAYING)
  {
    digitalWrite(START_LED1, LOW);
    digitalWrite(START_LED2, LOW);
    // Time to do the actual movement.
    // But, only when we are still playing. (We might have just lost ;))
    if (mode == PLAYING)
    {
      movement mv;
      mv.along = HIGH;
      mv.ashort = HIGH;
      mv.aplayer1 = LOW;
      mv.aplayer2 = LOW;

      digitalWrite(LONG_AXIS_DIR, current_long_direction);
      digitalWrite(SHORT_AXIS_DIR, current_short_direction);
      int p1 = 0;
      int p2 = 0;

      if (digitalRead(PLAYER1_LEFT) == HIGH && readStop(aplayer1, start) == MISSED)
      {
        digitalWrite(PLAYER1_DIR, HIGH);
        mv.aplayer1 = HIGH;
        p1 = -1;
      }
      else if (digitalRead(PLAYER1_RIGHT) == HIGH && readStop(aplayer1, end) == MISSED)
      {
        digitalWrite(PLAYER1_DIR, LOW);
        mv.aplayer1 = HIGH;
        p1 = 1;
      }

      if (digitalRead(PLAYER2_LEFT) == HIGH && readStop(aplayer2, end) == MISSED)
      {
        digitalWrite(PLAYER2_DIR, LOW);
        mv.aplayer2 = HIGH;
        p2 = 1;
      }
      else if (digitalRead(PLAYER2_RIGHT) == HIGH && readStop(aplayer2, start) == MISSED)
      {
        digitalWrite(PLAYER2_DIR, HIGH);
        mv.aplayer2 = HIGH;
        p2 = -1;
      }
      moveSeveralMotorsOneStep(mv);

      player1_location = player1_location + p1;
      player2_location = player2_location + p2;
      long_axis_location = long_axis_location + (current_long_direction == HIGH ? 1 : -1);
      short_axis_location = short_axis_location + (current_short_direction == HIGH ? 1 : -1);

      // Check some of the locations we are at. We don't want negative locations and stuff.
      if (long_axis_location < 0)
      {
        long_axis_location = 0;
        logging("Warn: long axis went to < 0");
      }
      if (short_axis_location < 0)
      {
        short_axis_location = 0;
        logging("Warn: short axis went to < 0");
      }
      long_last_change++;
      if (long_last_change >= steps_min_needed)
      {
        if (readStop(along, start) == HIT || readStop(along, end) == HIT)
        {
          current_long_direction = current_long_direction == HIGH ? LOW : HIGH;
          logging("Something hit (LONG). Turning");
          long_last_change = 0;
        }
      }
      short_last_change++;
      if (short_last_change >= steps_min_needed)
      {
        if (readStop(ashort, start) == HIT || readStop(ashort, end) == HIT)
        {
          current_short_direction = current_short_direction == HIGH ? LOW : HIGH;
          logging("Something hit (SHORT). Turning");
          short_last_change = 0;
        }
      }

      if (readStop(aplayer1, start))
      {
        player1_location = 0;
      }
      if (readStop(aplayer2, start))
      {
        player2_location = 0;
      }
      if (player1_location < 0)
      {
        // We are at the begin. Set the location to 0
        player1_location = 0;
        logging("Warn: player1 axis went to < 0");
      }
      if (player2_location < 0)
      {
        // We are at the begin. Set the location to 0
        player2_location = 0;
        logging("Warn: player2 axis went to < 0");
      }
    }
  }
  else
  {
    digitalWrite(START_LED1, HIGH);
    digitalWrite(START_LED2, HIGH);

    if (digitalRead(START_BUTTON1) == HIGH || digitalRead(START_BUTTON2) == HIGH)
    {
      digitalWrite(START_LED1, LOW);
      digitalWrite(START_LED2, LOW);
      logging("User wants to start playing");
      mode = PLAYING;

      disableMotor(false);

      // Lets first move to start to get a good base point
      logging("Going to start now.");
      player1_location = moveMotorToStart(aplayer1);
      player2_location = moveMotorToStart(aplayer2);

      long_axis_location = moveMotorToStart(along);
      short_axis_location = moveMotorToStart(ashort);

      logging("Going to center position.");

      long loc = (PLAYER_LENGTH - (PLAYER_SIZE / 2l)) * STEPS_PER_MM;
      player1_location = moveMotorToLocation(aplayer1, loc, player1_location);
      player2_location = moveMotorToLocation(aplayer2, loc, player2_location);

      loc = (LONG_AXIS_LENGTH - (BLOCK_SIZE / 2l)) * STEPS_PER_MM;
      long_axis_location = moveMotorToLocation(along, loc, long_axis_location);

      loc = (SHORT_AXIS_LENGTH - (BLOCK_SIZE / 2l)) * STEPS_PER_MM;
      short_axis_location = moveMotorToLocation(ashort, loc, short_axis_location);

      // Decide to which directory we go to
      current_long_direction = (random(0, 500) % 2 == 0) ? HIGH : LOW;
      current_short_direction = (random(0, 500) % 2 == 0) ? HIGH : LOW;
      short_last_change = 0;
      long_last_change = 0;

      digitalWrite(LONG_AXIS_DIR, current_long_direction);
      digitalWrite(SHORT_AXIS_DIR, current_short_direction);

      disableMotor(true);
      // Lets get starting with warning the user that we are about to start ;)
      logging("Init done. Now lets warn the user");
      for (int i = 0; i < 10; i++)
      {
        digitalWrite(START_LED1, (i % 2 == 0) ? HIGH : LOW);
        digitalWrite(START_LED2, (i % 2 == 0) ? HIGH : LOW);
        delay(250);
      }
      digitalWrite(START_LED1, LOW);
      digitalWrite(START_LED2, LOW);
      disableMotor(false);
      logging("Lets get started");
    }
  }
}
