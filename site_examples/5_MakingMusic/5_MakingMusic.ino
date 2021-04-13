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
 * Experiment: Making Music With A Muscle
 * Link: https://backyardbrains.com/experiments/music
 * 
 * Ported By: Ben Antonellis
 * Date: February 8th, 2021
**/

#include "NeuroBoard.hpp"

NeuroBoard board;

#include <Keyboard.h>

#define NOTE_C4  262    											// These definitions specify the frequency of each note.
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

#define MAX 500        												// Maximum posible reading. >>>> TWEAK THIS VALUE!! <<<<
#define MIN 100        												// Minimum posible reading. >>>> TWEAK THIS VALUE!! <<<<

int channel = 1;	   												// Indicates what channel the music box is in.

int c4Note;
int c4Duration = 600; 												// Determines the duration of each note in channel 4.
int c4Scale[]{        												// Inclues all the notes you can play on channel 4.
  NOTE_F4, NOTE_G4, NOTE_A4, NOTE_C5
  };
int c2Note;
int c2Duration = 300; 												// Determines the duration of each note in channel 2.
int c2Scale[]{        												// Inclues all the notes you can play on channel 2.
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4
  };
int melody[] = {        											// Includes the notes in the melody for "Mary had a Little Lamb".
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_A4, 
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_C5, NOTE_C5, 
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_A4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4
};
int noteDurations[] = { 											// Determines the duration of each note in the melody.
  4, 4, 4, 4, 4, 4, 2, 4, 4, 2, 4, 4, 2, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 2
};
int distortion;   													// Used later for distoriting a melody.
int note;

int c5Scale[]{    													// Inclues all the notes you can play on channel 4.
  291, 308, 326, 346, 366, 388, 411
};
int c5Note;
int c5Duration = 300;    											// Determines the duration of each note in channel 2.

int thisNote = 0;   												// Used later for changing between channels.
int buttonState;
int oldButtonState;

int finalReading;
byte litLeds = 0;
byte multiplier = 1;
byte leds[] = {8, 9, 10, 11, 12, 13};

void setup() {

	board.startMeasurements();

}

void loop() {

	// Button crap here

	int* reading;													// Array to collect EMG signals from your arm.
	board.getSamples(&reading, 10);									// Pass array to NeuroBoard to populate with samples.
	for (int i = 0; i < 10; i++) {
		finalReading += reading[i];
	}
	delete[] reading;												// Free memory. REQUIRED FOR REALLOCATION.
	finalReading /= 10;
	for (int i = 0; i < MAX_LEDS; i++) {
		board.writeLED(i, OFF);
	}
	Serial.print(finalReading);
	Serial.print("\t");
	finalReading = constrain(finalReading, 0, MAX);
	litLeds = map(finalReading, 0, MAX, 0, MAX_LEDS);
	Serial.println(litLeds);
	for (int i = 0; i < litLeds; i++) {
		board.writeLED(i, ON);
	}

	// Channel 1

	if ((channel == 1) && (finalReading > MIN)) {
		tone(8, finalReading / 1.5, 100);							// Outputs a tone equal to the final reading divided by 1.5.
		delay(100);
	}

	// Channel 2

	if ((channel == 2) && (finalReading > MIN)) {
		c2Note = map(finalReading, MIN, MAX, 0, 6);					// Maps frequency to a number between 0 and 6.
		tone(8, c2Scale[c2Note], c2Duration);						// Converts number to a note and plays it.
		delay(c2Duration);
	}

	// Channel 3

	if ((channel == 3) && (finalReading > MIN)) {
		c2Note = map(finalReading, MIN, MAX, 0, 6);					// Maps frequency to a number between 0 and 6.
		tone(8, c2Scale[c2Note] * 0.4 + 8, c2Duration);				// Converts number to a note and plays it.
		delay(c2Duration);
	}

	// Channel 4

	if ((channel == 4) && (finalReading > MIN)) {
		c5Note = map(finalReading, MIN, MAX, 0, 6);					// Maps frequency to a number between 0 and 6.
		tone(8, c5Scale[c5Note], c5Duration);						// Converts number to a note and plays it.
		delay(c5Duration);
	}

	// Channel 5

	if (channel == 5) {
		int noteDuration = (1400 - finalReading * 2.5) / noteDurations[thisNote];
		tone(8, melody[thisNote], noteDuration);
		delay(noteDuration * 1);									// Delay to distinguish between notes.
		noTone(8);
		if (thisNote == 24) {										// If melody is done, go to first note.
			delay(400);
			thisNote = -1;
		}
		thisNote++;
	}

	// Channel 6

	if (channel == 6) {
		int distortion = map(finalReading, MIN, MAX, 7, 14);
		Serial.println(distortion);
		int noteDuration = 1000 / noteDurations[thisNote];
		tone(8, (melody[thisNote] * distortion) / 10, noteDuration);
		delay(noteDuration * 1);									// Delay to distinguish between notes.
		noTone(8);
		if (thisNote == 24) {										// If melody is done, go to first note.
			delay(400);
			thisNote = -1;
		}
		thisNote++;
	}

}