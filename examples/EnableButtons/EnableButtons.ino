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
 * Simple program to demonstrate how to set buttons properly.
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

	// Set a trigger for a regular various button presses (RED or WHITE)
	
	board.enableButtonPress(WHITE_BTN, []() {
		Serial.println("White Button Pressed!");
	});
	
	board.enableButtonPress(RED_BTN, []() {
		Serial.println("Red Button Pressed!");
	});
	
	board.enableButtonLongPress(WHITE_BTN, 1000, []() {
    	Serial.println("White Button Held For 1 Second!");
	});

	board.enableButtonLongPress(RED_BTN, 1000, []() {
    	Serial.println("Red Button Held For 1 Second!");
	});
	
}

void loop() {

	// Required if any button/envelopeTrigger/servo is enabled
	board.handleInputs();

	// loop code here

}