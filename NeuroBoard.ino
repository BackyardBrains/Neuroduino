/**
 * NeuroBoard Test Implementation File.
 * All code in this file is subject to change.
 * 
 * @author Ben Antonellis
 * @date January 7th, 2021
**/

#include "NeuroBoard.hpp"

NeuroBoard board;

void setup() {

	board.startMeasurements();
	board.setChannel(A0);
	board.setDecayRate(10);
	board.startServo();
	
	board.enableButtonPress(WHITE_BTN, []() {
		Serial.println("white press");
	});
	
	board.enableButtonPress(RED_BTN, []() {
		Serial.println("red press");
	});
	
	board.enableButtonLongPress(WHITE_BTN, 1000, []() {
		Serial.println("white long press");
	});

	board.enableButtonLongPress(RED_BTN, 1000, []() {
		Serial.println("red long press");
	});

	board.setTriggerOnEnvelope(800, [](){
		Serial.println("Relay Activated!");
	});

	board.setServoDefaultPosition(CLOSED_MODE);
	board.displayEMGStrength();

}

void loop() {

	board.handleInputs();

	// Sample testing //

	int sample = board.getNewSample();
	int ev = board.getEnvelopeValue();

	Serial.println(sample);
	delay(5);

	//int* samples;
	//board.getSamples(&samples, 10);
	//for (int i = 0; i < 10; i++) {
	//	Serial.println(samples[i]);
	//}
	//delete[] samples; // Free memory for reallocation. REQUIRED.
	//delay(2000);

}
