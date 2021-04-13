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
 * Simple program that prints when a threshold is reached.
 * 
 * MAC:
 * 	View Console: Shift + Command + M
 * 	View Plotter: Shift + Command + L
 * 
 * WINDOWS:
 * 	View Console: Control + Shift + M
 *  View Plotter: Control + Shift + L
 * 
 * @author Ben Antonellis
 * @date January 7th, 2021
**/

#include "NeuroBoard.hpp"

NeuroBoard board;

void setup() {

	// Required to start receiving samples from the board //
	board.startMeasurements();

	// Once the incoming sample reaches above set threshold (700 in this case), the
	// function passed is called. Then, once the samples reached 9/10th of the passed
	// threshold (630 in this case), the function will be allowed to call again.

	// Once the threshold is met, the relay is turned on. It is only turned off when
	// the incoming samples are below the second threshold.

	board.setTriggerOnEnvelope(700, []() {
		Serial.println("Threshold Reached!");
	});

	// You can also set your own second threshold. The following code executes the function once
	// 600 is reached. Once the samples drop to 400 or below, only then will the functon be able
	// to be called again once hitting 600.

	board.setTriggerOnEnvelope(600, 400, []() {
		Serial.println("Threshold Reached!");
	});

}

void loop() {

	// Required if any button/envelopeTrigger/servo is enabled
	board.handleInputs();

	// loop code here

}