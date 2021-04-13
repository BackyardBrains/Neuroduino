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
 * Simple program that prints the signal recieved from the
 * spike recorder to the Arduino Serial Console/Plotter
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

}

void loop() {

	int sample = board.getNewSample();
	int envelopeValue = board.getEnvelopeValue();

	Serial.println(sample);

}