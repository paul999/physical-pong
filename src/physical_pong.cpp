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

void setup()
{
  Serial.begin(9600);
  logging("Starting up");

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

  logging("Going to location");
  long loc = (PLAYER_LENGTH - (PLAYER_SIZE / 2l)) * STEPS_PER_MM;
  player1_location = moveMotorToLocation(PLAYER1_STEP, PLAYER1_DIR, PLAYER1_BEGIN, PLAYER1_END, loc, player1_location);
  logging("Player2");
  player2_location = moveMotorToLocation(PLAYER2_STEP, PLAYER2_DIR, PLAYER2_BEGIN, PLAYER2_END, loc, player2_location);

  logging("Moving axis");
  long_axis_location = moveMotorToStart(LONG_AXIS_STEP, LONG_AXIS_DIR, LONG_AXIS_BEGIN, LONG_AXIS_END);
  short_axis_location = moveMotorToStart(SHORT_AXIS_STEP, SHORT_AXIS_DIR, SHORT_AXIS_BEGIN, SHORT_AXIS_END);

  loc = (LONG_AXIS_LENGTH - (BLOCK_SIZE / 2l)) * STEPS_PER_MM;
  long_axis_location = moveMotorToLocation(LONG_AXIS_STEP, LONG_AXIS_DIR, LONG_AXIS_BEGIN, LONG_AXIS_END, loc, long_axis_location);
  loc = (SHORT_AXIS_LENGTH - (BLOCK_SIZE / 2l)) * STEPS_PER_MM;
  short_axis_location = moveMotorToLocation(SHORT_AXIS_STEP, SHORT_AXIS_DIR, SHORT_AXIS_BEGIN, SHORT_AXIS_END, loc, short_axis_location);

  disableMotor(true);

  logging("Startup done. Now time to startup the loop");
}

void loop()
{
  //  return;
  if (mode == PLAYING)
  {
    logging("Playing");
    digitalWrite(START_LED1, LOW);
    digitalWrite(START_LED2, LOW);
  }
  else
  {
    digitalWrite(START_LED1, HIGH);
    digitalWrite(START_LED2, HIGH);
    logging("Not playing");

    if (digitalRead(START_BUTTON1) == HIGH || digitalRead(START_BUTTON2) == HIGH)
    {
      digitalWrite(START_LED1, LOW);
      digitalWrite(START_LED2, LOW);
      logging("User wants to start playing");
      mode = PLAYING;

      disableMotor(false);

      long loc = (PLAYER_LENGTH - (PLAYER_SIZE / 2l)) * STEPS_PER_MM;
      player1_location = moveMotorToLocation(PLAYER1_STEP, PLAYER1_DIR, PLAYER1_BEGIN, PLAYER1_END, loc, player1_location);
      player2_location = moveMotorToLocation(PLAYER2_STEP, PLAYER2_DIR, PLAYER2_BEGIN, PLAYER2_END, loc, player2_location);

      loc = (LONG_AXIS_LENGTH - (BLOCK_SIZE / 2l)) * STEPS_PER_MM;
      long_axis_location = moveMotorToLocation(LONG_AXIS_STEP, LONG_AXIS_DIR, LONG_AXIS_BEGIN, LONG_AXIS_END, loc, long_axis_location);
      loc = (SHORT_AXIS_LENGTH - (BLOCK_SIZE / 2l)) * STEPS_PER_MM;
      short_axis_location = moveMotorToLocation(SHORT_AXIS_STEP, SHORT_AXIS_DIR, SHORT_AXIS_BEGIN, SHORT_AXIS_END, loc, player2_location);

      disableMotor(true);
      // Lets get starting with warning the user that we are about to start ;)
      logging("Init done. Now lets warn the user");
      for (int i = 0; i < 10; i++)
      {
        digitalWrite(START_LED1, (i % 2 == 0) ? HIGH : LOW);
        digitalWrite(START_LED2, (i % 2 == 0) ? HIGH : LOW);
        delay(1000);
      }
      digitalWrite(START_LED1, LOW);
      digitalWrite(START_LED2, LOW);
      logging("Lets get started");
    }
  }
}
