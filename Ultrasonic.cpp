/*
 * Ultrasonic.cpp
 * A library for ultrasonic ranger
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : LG, FrankieChu
 * Create Time: Jan 17,2013
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdio.h>
#include <inttypes.h>
#include "Arduino.h"
#include "Ultrasonic.h"

#define NO_ECHO 0
const long ECHO_MAX_DIST_CM = 400l;
const long ECHO_MAX_DIST_INCH = ECHO_MAX_DIST_CM * 29 / 74;

#define ReadPin(x) digitalRead(x)
#define WritePin(x) digitalWrite(x)
#define MicrosecondsNow() micros()
#define SetPinMode(pin, mode) pinMode(pin, mode)
#define ON HIGH
#define OFF LOW
#define IN INPUT
#define OUT OUTPUT

static long pulseIn(long pin, long timeout)
{
	long begin = MicrosecondsNow();
	
	// wait for any previous pulse to end
	while (ReadPin(pin)) if (MicrosecondsNow() - begin >= timeout) return NO_ECHO;
	
	// wait for the pulse to start
	while (!ReadPin(pin)) if (MicrosecondsNow() - begin >= timeout) return NO_ECHO;
	long pulseBegin = MicrosecondsNow();
	
	// wait for the pulse to stop
	while (ReadPin(pin)) if (MicrosecondsNow() - begin >= timeout) return NO_ECHO;
	long pulseEnd = MicrosecondsNow();
	
	return pulseEnd - pulseBegin;
}

Ultrasonic::Ultrasonic(int pin)
{
	_pin = pin;
}

long Ultrasonic::MeasureDurationMicroseconds(long timeout = ECHOTIMEMAX)
	SetPinMode(_pin, OUT);
	WritePin(_pin, OFF);
	delayMicroseconds(2);
	WritePin(_pin, ON);
	delayMicroseconds(5);
	WritePin(_pin, OFF);
	SetPinMode(_pin,IN);
	return pulseIn(_pin, timeout);
}

/*The measured distance from the range 0 to 400 Centimeters*/
long Ultrasonic::MeasureInCentimeters(long timeout_distance = ECHO_MAX_DIST_CM)
{
	return MeasureDurationMicroseconds(timeout_distance*29*2)/29/2;
}
/*The measured distance from the range 0 to 157 Inches*/
long Ultrasonic::MeasureInInches(long timeout_distance = ECHO_MAX_DIST_INCH)
{
	return MeasureDurationMicroseconds(timeout_distance*74*2)/74/2;
}

/*Possibly add median:
meausure N times
remove all NO_ECHO values
sort
take median value
*/