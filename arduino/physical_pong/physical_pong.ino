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


void setup() {
  #ifdef debug 
    Serial.open(9600);
    log("Starting up");
  #endif
  pinMode(START_BUTTON, INPUT);
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
  pinMode(PLAYER2_DIR, OUTPUT);
  pinMode(PLAYER2_STEP, OUTPUT);
  pinMode(LONG_AXIS_DIR, OUTPUT);
  pinMode(LONG_AXIS_STEP, OUTPUT);
  pinMode(SHORT_AXIS_DIR, OUTPUT);
  pinMode(SHORT_AXIS_STEP, OUTPUT);
  pinMode(START_LED, OUTPUT);
  log("Startup done");
}

void log(String text) {
  #ifdef debug
  Serial.writeln(text);
  #endif
}

void loop() {
  if (mode == PLAYING){
    log("Playing");
    digitalWrite(START_LED, LOW);
  }
  else
  {
    digitalWrite(START_LED, HIGH);
    log("Not playing");

    if (digitalRead(START_BUTTON) == HIGH) {
      log("User wants to start playing");
    }
  }

}
