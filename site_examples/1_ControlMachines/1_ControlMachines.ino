/**
	NeuroBoard - A library for interacting with the Neuroduino Board.
    Copyright (C) 2021 Backyard Brains

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
    USA
    
	Backyard Brains hereby disclaims all copyright interest in the library
	`NeuroBoard` (a library for interacting with the Neuroduino Board) written
	by Benjamin Antonellis.

	Backyard Brains, March 2021
**/

/**
 * Experiment: Control Machines With Your Brain
 * Link: https://backyardbrains.com/experiments/muscleSpikerShield
 * 
 * Ported By: Ben Antonellis
 * Date: February 4th, 2021
**/

#include "NeuroBoard.hpp"

NeuroBoard board;

#define MAX 150
int readings[10];
int finalReading;
byte litLEDS = 0;
byte multiplier = 1;

void setup() {

	// Required for gathering samples from the board
    board.startMeasurements();

}

void loop() {

	// Get 10 readings from the board in ~0.02s
    for (int i = 0; i < 10; i++) {
        readings[i] = board.getNewSample() * multiplier;
        finalReading += readings[i];
        delay(2);
    }

	// Average out readings
    finalReading /= 10;

	// Turn off all LEDs
    for (int i = 0; i < MAX_LEDS; i++) {
        board.writeLED(i, OFF);
    }

	// Print finalReading to serial console
    Serial.print(finalReading);
    Serial.print("\t");

	// Constrain and map finalReading to get litLEDs
    finalReading = constrain(finalReading, 0, MAX);
    litLEDS = map(finalReading, 0, MAX, 0, MAX_LEDS);

	// Print litLEDS to serial console
    Serial.println(litLEDS);

	// Turn on number of LEDs determined by litLEDS
    for (int i = 0; i < litLEDS; i++) {
        board.writeLED(i, ON);
    }

}