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
#include "Arduino.h"
#include "display.h"
#include "log.h"

#if defined(ENABLE_SCORE_DISPLAY) && ENABLE_SCORE_DISPLAY == true

static const struct segmentNumber
{
  uint8_t d;
  uint8_t c;
  uint8_t b;
  uint8_t a;
} segmentNumber[] = {
    {LOW, LOW, LOW, LOW},    /* 0 */
    {LOW, LOW, LOW, HIGH},   /* 1 */
    {LOW, LOW, HIGH, LOW},   /* 2 */
    {LOW, LOW, HIGH, HIGH},  /* 3 */
    {LOW, HIGH, LOW, LOW},   /* 4 */
    {LOW, HIGH, LOW, HIGH},  /* 5 */
    {LOW, HIGH, HIGH, LOW},  /* 6 */
    {LOW, HIGH, HIGH, HIGH}, /* 7 */
    {HIGH, LOW, LOW, LOW},   /* 8 */
    {HIGH, LOW, LOW, HIGH},  /* 9 */
};

void setupSegmentPins()
{
  pinMode(SEGMENT_A_PIN, OUTPUT);
  pinMode(SEGMENT_B_PIN, OUTPUT);
  pinMode(SEGMENT_C_PIN, OUTPUT);
  pinMode(SEGMENT_D_PIN, OUTPUT);
  pinMode(PLAYER1_LATCH_PIN, OUTPUT);
  pinMode(PLAYER2_LATCH_PIN, OUTPUT);
  logging("Pin setup done for display");

  // Lets do a display test :)
  for (int i = 0; i < 10; i++)
  {
    setScore(p1, 9 - i);
    setScore(p2, i);
  }
  logging("Display test done");
  setScore(p1, 0);
  delay(250);
  setScore(p2, 0);
}
void setScore(player pl, int segment)
{
  if (segment > 9)
  {
    segment = 9; // We only have 1 segment, so set it to 9
  }
  digitalWrite(PLAYER1_LATCH_PIN, HIGH);
  digitalWrite(PLAYER2_LATCH_PIN, HIGH);
  delay(250);

  digitalWrite(SEGMENT_A_PIN, segmentNumber[segment].a);
  digitalWrite(SEGMENT_B_PIN, segmentNumber[segment].b);
  digitalWrite(SEGMENT_C_PIN, segmentNumber[segment].c);
  digitalWrite(SEGMENT_D_PIN, segmentNumber[segment].d);

  switch (pl)
  {
  case p1:
    digitalWrite(PLAYER1_LATCH_PIN, LOW);
    digitalWrite(PLAYER2_LATCH_PIN, HIGH);
    break;
  case p2:
    digitalWrite(PLAYER1_LATCH_PIN, HIGH);
    digitalWrite(PLAYER2_LATCH_PIN, LOW);
    break;
  }
  delay(250);

  digitalWrite(PLAYER1_LATCH_PIN, HIGH);
  digitalWrite(PLAYER2_LATCH_PIN, HIGH);
}
#endif