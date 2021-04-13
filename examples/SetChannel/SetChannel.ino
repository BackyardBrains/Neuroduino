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
 * Program to demonstrate the uses and limitations of the setChannel function.
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

	// There are a couple ways to set the channel to listen on. This is because
	// not everyone may be familiar with how channels are named.

	// To those unfamiliar with the Arduino and their analog naming
	// system, you can set the channel like so:

	board.setChannel(0); // Listens on A0, which is the 1st analog.
	board.setChannel(5); // Listens on A5, which is the 6th analog.

	// Any number N, where 0 <= N <= 11, will work.

	// To those who are familiar with Arduino's analog names, you can pass them
	// directly in to the function.
	
	board.setChannel(A0); // Listens on A0, which is the 1st analog.
	board.setChannel(A5); // Listens on A5, which is the 6th analog.
	
	// Any AN, where 0 <= N <= 11, will work.

	// For the best results, I recommend sticking with the A0 - A5 instead of using just 0 - 5.

	board.setChannel(A0);

}

void loop() {

	// loop code here

}