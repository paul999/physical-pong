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
#include "log.h"
#include <FastLED.h>
#include "player.h"

#if defined(ENABLE_LED_STRIP) && ENABLE_LED_STRIP == true

CRGB p1_leds[NUM_LEDS_PLAYER];
CRGB p2_leds[NUM_LEDS_PLAYER];
CRGB left_leds[NUM_LEDS_SIDE];
CRGB right_leds[NUM_LEDS_SIDE];

void allColor(CRGB value)
{
  for (int i = 0; i < NUM_LEDS_SIDE; i++)
  {
    if (i < NUM_LEDS_PLAYER)
    {
      p1_leds[i] = value;
      p2_leds[i] = value;
    }
    left_leds[i] = value;
    right_leds[i] = value;
  }
  FastLED.show();
}
void playersColor(CRGB value)
{
  for (int i = 0; i < NUM_LEDS_PLAYER; i++)
  {
    p1_leds[i] = value;
    p2_leds[i] = value;
  }
  FastLED.show();
}

void playerColor(CRGB value, player pl)
{
  for (int i = 0; i < NUM_LEDS_PLAYER; i++)
  {
    switch (pl)
    {
    case p1:
      p1_leds[i] = value;
      break;
    case p2:
      p2_leds[i] = value;
      break;
    }
  }
  FastLED.show();
}

void setupLedstrip()
{
  FastLED.setMaxPowerInVoltsAndMilliamps(5, LEDSTRIP_MAX_POWER);
  FastLED.setBrightness(255);
  FastLED.addLeds<LED_TYPE, LED_STRIP_P1_PIN, COLOR_ORDER>(p1_leds, NUM_LEDS_PLAYER);
  FastLED.addLeds<LED_TYPE, LED_STRIP_P2_PIN, COLOR_ORDER>(p2_leds, NUM_LEDS_PLAYER);
  FastLED.addLeds<LED_TYPE, LED_STRIP_LEFT_PIN, COLOR_ORDER>(left_leds, NUM_LEDS_SIDE);
  FastLED.addLeds<LED_TYPE, LED_STRIP_RIGHT_PIN, COLOR_ORDER>(right_leds, NUM_LEDS_SIDE);
  allColor(CRGB::Red);
}

void playerDead(player pl)
{
  CRGB p1_c;
  CRGB p2_c;
  switch (pl)
  {
  case p1:
    p1_c = CRGB::Red;
    p2_c = CRGB::Green;
    break;
  case p2:
    p1_c = CRGB::Green;
    p2_c = CRGB::Red;
    break;
  }
  for (int i = 0; i < NUM_LEDS_PLAYER; i++)
  {
    p1_leds[i] = p1_c;
    p2_leds[i] = p2_c;
  }
  FastLED.show();
}

void playersGreen()
{
  playersColor(CRGB::Green);
}

void playersWhite()
{
  playersColor(CRGB::White);
}

void allGreen()
{
  allColor(CRGB::Green);
}

void allWhite()
{
  allColor(CRGB::White);
}

void allRed()
{
  allColor(CRGB::Red);
}

#endif