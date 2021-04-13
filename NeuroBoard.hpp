/**
    NeuroBoard.hpp - A library for interacting with the Neuroduino Board.
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

    Backyard Brains, March 2021.
**/

/**
 * NeuroBoard Header File.
 * 
 * @author Ben Antonellis
 * @date January 7th, 2021
**/

#pragma once

#ifndef NEUROBOARD_HPP
#define NEUROBOARD_HPP

#include "Arduino.h"
#include <Servo.h>

// Defines //

#define NEUROBOARD_VERSION 	0.8
#define RED_BTN         	DD4
#define WHITE_BTN       	DD7
#define ON              	HIGH
#define OFF             	LOW
#define BUFFER_SIZE     	20
#define SERIAL_CAP      	230400

#ifdef ARDUINO_AVR_UNO
    #define MAX_LEDS 6
#else // We must be dealing with a Leonardo
    #define MAX_LEDS 8
#endif

typedef unsigned long ulong;

// analogRead macros //

#ifndef _BV
    #define _BV(bit) (1 << (bit))
#endif
#ifndef cbi
    #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) // Clear bit.
#endif
#ifndef sbi
    #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit)) // Set bit.
#endif

// Servo Code Start //

/**
  * Written by Marcio Amorim
  * Updated by Stanislav Mircic
  * Ported/Updated by Ben Antonellis
**/

#define RELAY_PIN                 3             // Pin for relay that controls TENS device
#define RELAY_THRESHOLD           4             // Defines sensitivity of relay
#define SERVO_PIN                 2             // Pin for servo motor
#define NUM_LED                   6             // Number of LEDs in LED bar
#define GRIPPER_MINIMUM_STEP      5             // 5 degree dead zone (used to avoid aiming oscilation)
#define OPEN_MODE                 1             // Default gripper state is opened
#define CLOSED_MODE               2             // Default gripper state is closed
#define MINIMUM_SERVO_UPDATE_TIME 100           // Update servo position every 100ms

/**
 * Struct for maintaining the servo during its usage.
**/
struct NeuroServo {

    NeuroServo() {};

    Servo Gripper;                              // Servo for gripper
    
    //EMG saturation values (when EMG reaches this value the gripper will be fully opened/closed)
    int sensitivities[6] = {200, 350, 520, 680, 840, 1024};
    int lastSensitivitiesIndex = 4;             // Set initial sensitivity index
    
    int emgSaturationValue = 1024;              // Selected sensitivity/EMG saturation value
    int analogReadings;                         // Measured value for EMG
    byte ledbarHeight = 0;                      // Temporary variable for led bar height
    
    ulong oldTime = 0;                          // Timestamp of last servo angle update (ms)
    int oldDegrees = 0;                         // Old value of angle for servo
    int newDegree;                              // New value of angle for servo
    
    ulong debouncerTimer = 0;                   // Timer for button debouncer         
    int gripperStateButtonValue = 0;            // Temporary variable that stores state of button 
    int userReleasedButton = 1;                 // Flag that is used to avoid multiple button events when user holds button
    
    int currentFunctionality = CLOSED_MODE;     // Current default position of claw

};

// Servo Code End //

/**
 * Struct for better handling of buttons and their callbacks.
**/
struct Button {

    void (*callback)(void);
    unsigned int interval;
    bool enabled;

    Button() {};

    void set(void (*callback)(void), const int& interval, const bool& enabled) {
        this->callback = callback;
        this->interval = interval;
        this->enabled = enabled;
    }

};

/**
 * Struct for handling trigger events.
**/
struct Trigger {

    int threshold;
    int secondThreshold;
    void (*callback)(void);
    bool enabled;
    bool thresholdMet;

    Trigger() {};

    void set(const int& threshold, const int& secondThreshold, void (*callback)(void), const bool& enabled, const bool& thresholdMet) {
        this->threshold = threshold;
        this->secondThreshold = secondThreshold;
        this->callback = callback;
        this->enabled = enabled;
        this->thresholdMet = thresholdMet;
    }

};

/**
 * Class for interacting with the Neuroduino Board.
**/
class NeuroBoard {

    public:

        // Static member variables //

        static uint8_t channel;
        static int decayRate;

        /**
         * Samples data to a circular buffer, and calculates envelope value 
         * all in the background. Sets pin modes for Stanislav's code.
         * 
         * - Usable in setup: true
         * - Usable in loop: false
         * 
         * @return void.
        **/
        void startMeasurements(void);

        /**
         * Special function to handle all button triggers, envelope triggers, and
         * servo pings.
         * 
         * @return void.
        **/
        void handleInputs(void);

        /**
         * Sets up the servo for use with the NeuroBoard.
         * 
         * - Usable in setup: true
         * - Usable in loop: true
         * 
         * @return void.
        **/
        void startServo(void);

        /**
         * Detaches the servo from the NeuroBoard, ending communication.
         * 
         * - Usable in setup: true
         * - Usable in loop: true
        **/
        void endServo(void);

        /**
         * Increases sensitivity for the servo.
         * 
         * - Usable in setup: true
         * - Usable in loop: true
         * 
         * @return void.
        **/
        void increaseSensitivity(void);

        /**
         * Decreases sensitivity for the servo.
         * 
         * - Usable in setup: true
         * - Usable in loop: true
         * 
         * @return void.
        **/
        void decreaseSensitivity(void);

        /**
         * Toggles the servos default position, from open to closed.
         * 
         * @param position Either OPEN_MODE or CLOSED_MODE.
         * 
         * @return void.
        **/
        void setServoDefaultPosition(const int& position);

        /**
         * Returns the last measured sample from the channel.
         * 
         * - Usable in setup: false
         * - Usable in loop: true
         * 
         * @return int - Last measured sample.
        **/
        int getNewSample(void);

		/**
		 * Returns "size" samples to the passed array.
		 * 
		 * - Usable in setup: false
		 * - Usable in loop: true
		 * 
		 * Example Code:
		 * 
		 * void setup() {
		 * 
		 *     board.startMeasurements();
		 * 
		 * }
		 * 
		 * void loop() {
		 * 
		 *     int* samples;
		 *     board.getSamples(&samples, 10);
		 *     delete[] samples; // Frees memory for reallocation. REQUIRED.
		 * 
		 * }
		 * 
		 * @param arr Array to modify.
		 * @param size Number of samples to add (should be same size as arr).
		 * 
		 * @return void.
		**/
		void getSamples(int* arr[], const int& size);

        /**
         * Returns the envelope value of the channel.
         * 
         * For every incoming reading from the analog, if that reading isn't bigger
         * than the current envelope value, the envelope value is subtracted by one.
         * This ensures we don't have an envelope value higher than necessary.
         * 
         * - Usable in setup: false
         * - Usable in loop: true
         * 
         * @return int - Envelope value.
        **/
        int getEnvelopeValue(void);

        /**
         * Sets the current channel to listen on. Works with A0, A1, A2 etc.
		 * 
         * Boundries for built in analogs:
         *      setChannel(n) where:
         *          A0 <= n <= A5
         * 
         * - Usable in setup: true
         * - Usable in loop: true
         * 
         * @param uint8_t Channel.
         * 
         * @return void.
        **/
        void setChannel(const uint8_t& channel);

        /**
         * Sets the decay rate for the setTriggerOnEnvelope function.
         * I.E, setDecayRate(5) will subtract envelopeValue by 5 every tick.
         * 
         * - Usable in setup: true
         * - Usable in loop: true
         * 
         * @param rate Integer representing the scale at which envelopeValue will be subtracted.
         * 
         * @return void.
        **/
        void setDecayRate(const int& rate);

        /**
         * Calls the passed function when the specified button is pressed.
         * 
         * - Usable in setup: true
         * - Usable in loop: false
         * 
         * @param button Which button to map the passed function to.
         * @param interval Delay between each button press to accept input.
         * @param callback Function to call when button is pressed.
         * 
         * @return void.
        **/
        void enableButtonPress(const uint8_t& button, const int& interval, void (*callback)(void));

        /**
         * Calls the passed function when the specified button is pressed.
         * 
         * - Usable in setup: true
         * - Usable in loop: false
         * 
         * @param button Which button to map the passed function to.
         * @param callback Function to call when button is pressed.
        **/
        void enableButtonPress(const uint8_t& button, void (*callback)(void));

        /**
         * Calls a function when a button is pressed for a long time.
         * 
         * - Usable in setup: true
         * - Usable in loop: false
         * 
         * @param button Which button to map the passed function to.
         * @param milliseconds How many milliseconds the user should hold button for to execute function.
         * @param callback Function to call when button is held for X milliseconds.
         * 
         * @return void.
        **/
        void enableButtonLongPress(const uint8_t& button, const int& milliseconds, void (*callback)(void));

        /**
         * Calls the passed function when the envelope value is greater 
         * than the passed threshold.
         * 
         * - Usable in setup: false
         * - Usable in loop: false
         * 
         * @param threshold Threshold for envelope value.
         * @param secondFactor Optional parameter for the second threshold the data must pass.
         * @param callback Function to call when threshold is reached.
         * 
         * @return void.
        **/
        void setTriggerOnEnvelope(const int& threshold, const int& secondFactor, void (*callback)(void));

        /**
         * Calls the passed function when the envelope value is greater
         * than the passed threshold.
         * 
         * - Usable in setup: false
         * - Usable in loop: false
         * 
         * @param threshold Threshold for envelope value.
         * @param callback Function to call when threshold is reached.
         * 
         * @return void.
        **/
        void setTriggerOnEnvelope(const int& threshold, void (*callback)(void));

        /**
         * Sets a flag to display the current strength of the readings using the
         * LED bar.
         * 
         * - Usable in setup: false
         * - Usable in loop: true
         * 
         * @return void.
        **/
        void displayEMGStrength(void);

        /* ******************************************************* */
        /** @author Stanislav Mircic **/

        void writeLED(const int& led, const bool& state);

        /* ******************************************************* */

    private:

        /* ******************************************************* */
        /** @author Stanislav Mircic **/

        void writeLEDs(void);
        void writeLEDs(byte outByte);
        byte _shiftRegState = 0;

        /* ******************************************************* */

        /**
         * Available channels to listen to. This is board specific.
         * Neuroduino (Arduino Leonardo).
        **/
        #ifdef ARDUINO_AVR_UNO
            int channels[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
        #else // We must be dealing with a Leonardo
            int channels[6] = {A0, A1, A2, A3, A4, A5};
        #endif

        /**
         * LED pins for different boards.
        **/
        #ifdef ARDUINO_AVR_UNO
            int ledPins[6] = {8, 9, 10, 11, 12, 13};
        #else // We must be dealing with a Leonardo
            int ledPins[8] = {0, 1, 2, 3, 4, 5, 6, 7};
        #endif

};

/**
 * Custom delay function so our code can continue to run while the 
 * user wants to delay. This is non-blocking, so code will continue
 * to run.
 * 
 * - Usable in setup: false
 * - Usable in loop: true
 * 
 * @param milliseconds How many milliseconds the user wants to delay.
 * @param variable Variable to use to hold count.
 * 
 * @return void.
**/
bool wait(const int& milliseconds, ulong& variable);

#endif // NEUROBOARD_HPP
