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
 * Simple program that controls the servo with analog readings.
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

	// **************************************** //
	// OPTION 1 //

	// You can start the servo by simplying typing this line in the setup function

	board.startServo();

	// **************************************** //
	// OPTION 2 //

	// Alternatively, we could set the servo based on a button pressed //

	board.enableButtonPress(WHITE_BTN, []() {
		board.startServo();
	});

	// **************************************** //
	// OPTION 3 //

	// We could also start the servo, and set sensitivity calls to the buttons //

	board.startServo();

	board.enableButtonPress(RED_BTN, []() {
		board.increaseSensitivity();
	});

	board.enableButtonPress(WHITE_BTN, []() {
		board.decreaseSensitivity();
	});

	// **************************************** //
	// OPTION 4 //

	// We could start the servo, and set the buttons to change the default servo mode //

	board.startServo();

	board.enableButtonPress(RED_BTN, []() {
		board.setServoDefaultPosition(OPEN_MODE);
	});

	board.enableButtonPress(WHITE_BTN, []() {
		board.setServoDefaultPosition(CLOSED_MODE);
	});

	// **************************************** //

}

void loop() {

	// Required if any button/envelopeTrigger/servo is enabled
	board.handleInputs();

	// If we want a visual representation of EMG strength, we can call the below function //

	board.displayEMGStrength();

	// loop code here

}