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
#include "log.h"

long player1_location = 0;
long player2_location = 0;
long short_axis_location = 0;
long long_axis_location = 0;
int mode = WAITING_FOR_START;
int current_long_direction = 0;
int current_short_direction = 0;

bool short_changed_previous = false;
bool long_changed_previous = false;

void setup()
{
  Serial.begin(9600);
  logging("Starting up");

  randomSeed(analogRead(0));

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
  logging("Startup done");

  disableMotor(false);

  logging("Going to start positions");

  player1_location = moveMotorToStart(PLAYER1_STEP, PLAYER1_DIR, PLAYER1_BEGIN, PLAYER1_END);
  player2_location = moveMotorToStart(PLAYER2_STEP, PLAYER2_DIR, PLAYER2_BEGIN, PLAYER2_END);

  logging("Moving axis");
  long_axis_location = moveMotorToStart(LONG_AXIS_STEP, LONG_AXIS_DIR, LONG_AXIS_BEGIN, LONG_AXIS_END);
  short_axis_location = moveMotorToStart(SHORT_AXIS_STEP, SHORT_AXIS_DIR, SHORT_AXIS_BEGIN, SHORT_AXIS_END);

  disableMotor(true);

  logging("Startup done. Now time to startup the loop");
}

void loop()
{
  if (mode == PLAYING)
  {
    logging("Playing");
    digitalWrite(START_LED1, LOW);
    digitalWrite(START_LED2, LOW);

    if (!long_changed_previous)
    {
      if (digitalRead(LONG_AXIS_BEGIN) == LOW)
      {
        // Check if player1 is in the right spot.
        // Todo: add checking code. For now we just go back
        current_long_direction = (current_long_direction == HIGH) ? LOW : HIGH;
        long_changed_previous = true;
      }
      else if (digitalRead(LONG_AXIS_END) == LOW)
      {
        // Check if player2 is in the right spot.
        // Todo: add checking code. For now we just go back
        current_long_direction = (current_long_direction == HIGH) ? LOW : HIGH;
        long_changed_previous = true;
      }
    }
    if (!short_changed_previous)
    {
      if (digitalRead(SHORT_AXIS_BEGIN) == LOW || digitalRead(SHORT_AXIS_END) == LOW)
      {
        current_short_direction = (current_short_direction == HIGH) ? LOW : HIGH;
      }
    }

    // Time to do the actual movement.
    // But, only when we are still playing. (We might have just lost ;))
    if (mode == PLAYING)
    {
      int number = 2;
      int motors[4];
      motors[0] = LONG_AXIS_STEP;
      motors[1] = SHORT_AXIS_STEP;
      digitalWrite(LONG_AXIS_DIR, current_long_direction);
      digitalWrite(SHORT_AXIS_DIR, current_short_direction);
      int p1 = 0;
      int p2 = 0;

      if (digitalRead(PLAYER1_LEFT) == HIGH)
      {
        // TODO: Set dir.
        motors[number] = PLAYER1_STEP;
        number++;
        p1 = -1;
      }
      else if (digitalRead(PLAYER1_RIGHT))
      {
        // TODO: Set dir.
        motors[number] = PLAYER1_STEP;
        number++;
        p1 = 1;
      }

      if (digitalRead(PLAYER2_LEFT) == HIGH)
      {
        // TODO: Set dir.
        motors[number] = PLAYER2_STEP;
        number++;
        p2 = 1;
      }
      else if (digitalRead(PLAYER2_RIGHT))
      {
        // TODO: Set dir.
        motors[number] = PLAYER2_STEP;
        number++;
        p2 = -1;
      }
      moveSeveralMotorsOneStep(number, motors);

      player1_location = player1_location + p1;
      player2_location = player2_location + p2;
      long_axis_location = long_axis_location + (current_long_direction == HIGH ? 1 : -1);
      short_axis_location = short_axis_location + (current_short_direction == HIGH ? 1 : -1);

      // Check some of the locations we are at. We don't want negative locations and stuff.
      if (long_axis_location < 0 || digitalRead(LONG_AXIS_BEGIN) == LOW)
      {
        // We are at the begin. Set the location to 0
        long_axis_location = 0;
      }
      if (short_axis_location < 0 || digitalRead(SHORT_AXIS_BEGIN) == LOW)
      {
        // We are at the begin. Set the location to 0
        short_axis_location = 0;
      }
      if (player1_location < 0 || digitalRead(PLAYER1_BEGIN) == LOW)
      {
        // We are at the begin. Set the location to 0
        player1_location = 0;
      }
      if (player2_location < 0 || digitalRead(PLAYER2_BEGIN) == LOW)
      {
        // We are at the begin. Set the location to 0
        player2_location = 0;
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
      player1_location = moveMotorToStart(PLAYER1_STEP, PLAYER1_DIR, PLAYER1_BEGIN, PLAYER1_END);
      player2_location = moveMotorToStart(PLAYER2_STEP, PLAYER2_DIR, PLAYER2_BEGIN, PLAYER2_END);

      long_axis_location = moveMotorToStart(LONG_AXIS_STEP, LONG_AXIS_DIR, LONG_AXIS_BEGIN, LONG_AXIS_END);
      short_axis_location = moveMotorToStart(SHORT_AXIS_STEP, SHORT_AXIS_DIR, SHORT_AXIS_BEGIN, SHORT_AXIS_END);

      logging("Going to center position.");

      long loc = (PLAYER_LENGTH - (PLAYER_SIZE / 2l)) * STEPS_PER_MM;
      player1_location = moveMotorToLocation(PLAYER1_STEP, PLAYER1_DIR, PLAYER1_BEGIN, PLAYER1_END, loc, player1_location);
      player2_location = moveMotorToLocation(PLAYER2_STEP, PLAYER2_DIR, PLAYER2_BEGIN, PLAYER2_END, loc, player2_location);

      loc = (LONG_AXIS_LENGTH - (BLOCK_SIZE / 2l)) * STEPS_PER_MM;
      long_axis_location = moveMotorToLocation(LONG_AXIS_STEP, LONG_AXIS_DIR, LONG_AXIS_BEGIN, LONG_AXIS_END, loc, long_axis_location);

      loc = (SHORT_AXIS_LENGTH - (BLOCK_SIZE / 2l)) * STEPS_PER_MM;
      short_axis_location = moveMotorToLocation(SHORT_AXIS_STEP, SHORT_AXIS_DIR, SHORT_AXIS_BEGIN, SHORT_AXIS_END, loc, short_axis_location);

      // Decide to which directory we go to
      current_long_direction = (random(0, 500) % 2 == 0) ? HIGH : LOW;
      current_short_direction = (random(0, 500) % 2 == 0) ? HIGH : LOW;

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
