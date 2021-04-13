/**
    NeuroBoard.cpp - A library for interacting with the Neuroduino Board.
    Copyright (C) 2021 Backyard Brains.

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
    USA.
    
    Backyard Brains hereby disclaims all copyright interest in the library
    `NeuroBoard` (a library for interacting with the Neuroduino Board) written
    by Benjamin Antonellis.

    Backyard Brains, March 2021.
**/

/**
 * NeuroBoard Implementation File.
 *
 * @author Ben Antonellis
 * @date January 7th, 2021
**/

#include "NeuroBoard.hpp"

/* ******************************************************* */
/** @author Stanislav Mircic **/

#define SHIFT_LATCH_PIN    B00000100                        // latch pin for shift register RCK - PB2
#define I_SHIFT_LATCH_PIN  B11111011
#define SHIFT_CLOCK_PIN    B00000010                        // clock pin for shift register PB1
#define I_SHIFT_CLOCK_PIN  B11111101
#define SHIFT_DATA_PIN     B00001000                        // serial data pin for shift register SER - PB3
#define I_SHIFT_DATA_PIN   B11110111
#define BITMASK_ONE        B00000001
#define I_BITMASK_ONE      B11111110

/* ******************************************************* */

/// PRIVATE FUNCTIONS ///

/**
 * Faster version of map() that doesn't use multiplication or division.
**/
long fasterMap(long value, long fromLow, long fromHigh, long toLow, long toHigh) {

    long first = value - fromLow;
    long second = toHigh - toLow;
    long combined = 0;
    while (second > 0) {                   // Avoid usage of * operator
        combined = combined + first;
        second = second - 1;
    }
    long third = fromHigh - fromLow;
    long count = 0;
    while (combined >= third) {            // Avoid usage of / operator
        combined = combined - third;
        count = count + 1;
    }
    count = count + toLow;
    return count;

}

// Button Wait Variables //

ulong redCount = 0;
ulong whiteCount = 0;
ulong redDebounceCount = 0;
ulong whiteDebounceCount = 0;

// Button Variables //

Button redButtonTrigger = Button();
Button whiteButtonTrigger = Button();
Button redLongButtonTrigger = Button();
Button whiteLongButtonTrigger = Button();

// Trigger Variable //

Trigger envelopeTrigger = Trigger();

// Servo Variable //

NeuroServo servo = NeuroServo();
bool servoEnabled = false;
bool emgStrengthEnabled = false;

int NeuroBoard::decayRate = 1;

// Buffer Variables //

int* buffer = new int[BUFFER_SIZE];
int head = 0;
int tail = 0;
bool full = false;

// Envelope Value //

int envelopeValue;

// Serial variable, changed in startCommunication method //

bool communicate = false;
uint8_t NeuroBoard::channel = A0;

// Variables for button holding //

int RBD = 0;                // Red Button Down
ulong RBT = 0;              // Red Button Time
int RBC = 0;                // Red Button Collected

int WBD = 0;                // White Button Down
ulong WBT = 0;              // White Button Time
int WBC = 0;                // White Button Collected

int redLongButtonHeld = 0;
int whiteLongButtonHeld = 0;

int redLongCalled = 0;
int whiteLongCalled = 0;

// Sample variable //

int reading;

// ISR //

ISR (TIMER3_COMPA_vect) {

    // Get reading from analog //

    reading = analogRead(NeuroBoard::channel);

    // Calculate envelope value here //

    envelopeValue = (reading >= envelopeValue) ? (reading) : (envelopeValue - NeuroBoard::decayRate);

    // Place new reading in buffer //

    buffer[head] = reading;

    if (full) {
        tail = (tail == BUFFER_SIZE) ? (0) : (tail + 1);
    }
    head = (head == BUFFER_SIZE) ? (0) : (head + 1);
    full = head == tail;

}

// PUBLIC METHODS //

void NeuroBoard::startMeasurements(void) {

    // Start Serial //

    Serial.begin(SERIAL_CAP);

    // Set pin modes for Stanislav's code //

    pinMode(14, OUTPUT); // MISO
    pinMode(15, OUTPUT); // SCK
    pinMode(16, OUTPUT); // MOSI

    // Set relay pin //

    pinMode(RELAY_PIN, OUTPUT);

    // Initialize timer //

    // Disable interrupts //

    noInterrupts();

    // Set prescale to 16 for analogRead

    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    cbi(ADCSRA, ADPS0);

    // Set timer register flags //

    TCCR3A = 0;
    TCCR3B = 0;
    TCNT3 = 0;

    // Configure timer registers //

    // Performance Upgrade //
    // 35 samples/second => 245 samples/second

    OCR3A = 31250;
    TCCR3B = (TCCR3B & 0xF8) | 0x01;
    TIMSK3 |= (1 << OCIE1A);

    // Enable interrupts //

    interrupts();

}

bool redPressed() { return PIND & B00010000; }
bool whitePressed() { return PINE & B01000000; }

void NeuroBoard::handleInputs(void) {

    // Check if buttons are enabled //

    if (redButtonTrigger.enabled) {
        if (redPressed()) {
            RBD = 1;
            if (!RBC) {
                RBT = millis();
                RBC = 1;
            }
        } else {
            if (RBD) {
                if ((millis() - RBT) <= 250) {
                    redButtonTrigger.callback();
                }
                RBD = 0;
                RBC = 0;
            }
        }
    }

    if (whiteButtonTrigger.enabled) {
        if (whitePressed()) {
            WBD = 1;
            if (!WBC) {
                WBT = millis();
                WBC = 1;
            }
        } else {
            if (WBD) {
                if ((millis() - WBT) <= 250) {
                    whiteButtonTrigger.callback();
                }
                WBD = 0;
                WBC = 0;
            }
        }
    }

    if (redLongButtonTrigger.enabled) {
        if (redPressed()) {
            if (redLongButtonHeld) {
                if (wait(redLongButtonTrigger.interval, redCount)) {
                    if (!redLongCalled) {
                        redLongButtonTrigger.callback();
                        redLongButtonHeld = 0;
                        redLongCalled = 1;
                    }
                }
            }
            redLongButtonHeld = 1;
        } else {
            redLongButtonHeld = 0;
            redLongCalled = 0;
            redCount = millis();
        }
    }

    if (whiteLongButtonTrigger.enabled) {
        if (whitePressed()) {
            if (whiteLongButtonHeld) {
                if (wait(whiteLongButtonTrigger.interval, whiteCount)) {
                    if (!whiteLongCalled) {
                        whiteLongButtonTrigger.callback();
                        whiteLongButtonHeld = 0;
                        whiteLongCalled = 1;
                    }
                }
            }
            whiteLongButtonHeld = 1;
        } else {
            whiteLongButtonHeld = 0;
            whiteLongCalled = 0;
            whiteCount = millis();
        }
    }

    // Check if envelope trigger is set //

    if (envelopeTrigger.enabled) {

        if (envelopeValue >= envelopeTrigger.threshold) {
            if (!envelopeTrigger.thresholdMet) {
                envelopeTrigger.thresholdMet = true;
                envelopeTrigger.callback();
                PORTD = PORTD | BITMASK_ONE;   // digitalWrite(RELAY_PIN, ON);
                delay(1);                      // Wait 1 ms to register relay pin as ON.
                PORTD = PORTD & I_BITMASK_ONE; // digitalWrite(RELAY_PIN, OFF);
            }
        } else {
            if (envelopeValue <= envelopeTrigger.secondThreshold) {
                envelopeTrigger.thresholdMet = false;
            }
        }

    }

    // Servo Code //

    if (servoEnabled) {

        servo.analogReadings = reading;

        // Set new angle if enough time passed
        if (millis() - servo.oldTime > MINIMUM_SERVO_UPDATE_TIME) {

            // Calculate new angle for servo
            if (servo.currentFunctionality == OPEN_MODE) {
                servo.analogReadings = constrain(servo.analogReadings, 40, servo.emgSaturationValue);
                servo.newDegree = fasterMap(servo.analogReadings, 40, servo.emgSaturationValue, 190, 105);
            } else {
                servo.analogReadings = constrain(servo.analogReadings, 120, servo.emgSaturationValue);
                servo.newDegree = fasterMap(servo.analogReadings, 120, servo.emgSaturationValue, 105, 190);
            }

            // Check if we are in servo dead zone
            if (abs(servo.newDegree - servo.oldDegrees) > GRIPPER_MINIMUM_STEP) {
                // Set new servo angle
                servo.Gripper.write(servo.newDegree);
            }

            // Set old time and degrees for new calculation
            servo.oldTime = millis();
            servo.oldDegrees = servo.newDegree;

        }

    }

    // EMG Strength Code //

    if (emgStrengthEnabled) {

        // Turn OFF all LEDs on LED bar
        for (int i = 0; i < MAX_LEDS; i++) {
            this->writeLED(this->ledPins[i], OFF);
        }

        // Calculate what LEDs should be turned ON on the LED bar
        int readings = constrain(reading, 30, servo.emgSaturationValue);
        servo.ledbarHeight = fasterMap(readings, 30, servo.emgSaturationValue, 0, MAX_LEDS);

        // Display fix for when servo is disabled, but user still wants visual feedback
        // Last check is for a Leonardo Board. Not tested with an Arduino Uno yet.
        if (!servoEnabled and servo.ledbarHeight == 7 and MAX_LEDS == 8) {
            servo.ledbarHeight++;
        }

        // Turn ON LEDs on the LED bar
        for (int i = 0; i < servo.ledbarHeight; i++) {
            this->writeLED(this->ledPins[i], ON);
        }

    }

}

void NeuroBoard::startServo(void) {

    // Ensure servo isn't already enabled before starting
    if (servoEnabled) return;

    // Attach servo to board
    servo.Gripper.attach(SERVO_PIN);

    PORTD = PORTD & I_BITMASK_ONE; // digitalWrite(RELAY_PIN, OFF);

    // Initialize all LED pins to output
    for (int i = 0; i < MAX_LEDS; i++) {
        pinMode(this->ledPins[i], OUTPUT);
    }

    // Get current sensitivity
    servo.emgSaturationValue = servo.sensitivities[servo.lastSensitivitiesIndex];

    // Set servo enabled boolean
    servoEnabled = true;

}

void NeuroBoard::endServo(void) {

    // Ensure servo is enabled before disabling
    if (!servoEnabled) return;

    // Set servo boolean value to false
    servoEnabled = false;

    // Detach servo
    servo.Gripper.detach();

    // Reset servo object to default values
    servo = NeuroServo();

}

void NeuroBoard::increaseSensitivity(void) {

    // Ensure servo is enabled before modifying sensitivity value
    if (!servoEnabled || servo.lastSensitivitiesIndex == 5) return; // 5 equals end of sensitivity array

    // Increment sensitivity index
    servo.lastSensitivitiesIndex++;

    // Get current sensitivity value
    servo.emgSaturationValue = servo.sensitivities[servo.lastSensitivitiesIndex];

}

void NeuroBoard::decreaseSensitivity(void) {

    // Ensure servo is enabled before modifying sensitivity value
    if (!servoEnabled || servo.lastSensitivitiesIndex == 0) return;

    // Decrement sensitivity index
    servo.lastSensitivitiesIndex--;

    // Get current sensitivity value
    servo.emgSaturationValue = servo.sensitivities[servo.lastSensitivitiesIndex];

}

void NeuroBoard::setServoDefaultPosition(const int& position) {

    servo.currentFunctionality = position;

}

int NeuroBoard::getNewSample(void) {

    int value = buffer[tail]; // Can't just return this because tail is changed below //
    full = false;
    tail = (tail == BUFFER_SIZE) ? (0) : (tail + 1);

    return value;

}

void NeuroBoard::getSamples(int* arr[], const int& size) {

    *arr = new int[size];
    for (int i = 0; i < size; i++) {
        // We can't just access the buffer directly because we need to modify it once retrieving a value.
        (*arr)[i] = this->getNewSample();
    }

}

int NeuroBoard::getEnvelopeValue(void) {

    return envelopeValue;

}

void NeuroBoard::setChannel(const uint8_t& newChannel) {

    NeuroBoard::channel = newChannel;

}

void NeuroBoard::setDecayRate(const int& rate) {

    // Check to ensure positive input, some users may interpret decay rate
    // as a negative value. This prevents that mistake.

    NeuroBoard::decayRate = (rate < 0) ? -rate : rate;

}

void NeuroBoard::enableButtonPress(const uint8_t& button, void (*callback)(void)) {

    if (button == RED_BTN) {
        redButtonTrigger.set(callback, 0, true);
    }

    if (button == WHITE_BTN) {
        whiteButtonTrigger.set(callback, 0, true);
    }

}

void NeuroBoard::enableButtonLongPress(const uint8_t& button, const int& milliseconds, void (*callback)(void)) {

    if (button == RED_BTN) {
        redLongButtonTrigger.set(callback, milliseconds, true);
    }

    if (button == WHITE_BTN) {
        whiteLongButtonTrigger.set(callback, milliseconds, true);
    }

}

void NeuroBoard::setTriggerOnEnvelope(const int& threshold, const int& secondFactor, void (*callback)(void)) {

    envelopeTrigger.set(threshold, secondFactor, callback, true, false);

}

void NeuroBoard::setTriggerOnEnvelope(const int& threshold, void (*callback)(void)) {

    this->setTriggerOnEnvelope(threshold, threshold - (threshold / 10), callback);

}

void NeuroBoard::displayEMGStrength(void) {

    emgStrengthEnabled = !emgStrengthEnabled;

}

bool wait(const int& milliseconds, ulong& variable) {

    ulong ms = millis();
    bool done = (ms - variable) >= milliseconds;
    if (done)
        variable = ms;
    return done;

}

/* ******************************************************* */
/** @author Stanislav Mircic **/
/** Simplified by Ben Antonellis **/

void NeuroBoard::writeLEDs(void) {

    PORTB &= I_SHIFT_LATCH_PIN;
    byte tempBitmask = BITMASK_ONE;

    for (int i = 0; i < MAX_LEDS; i++) {

        PORTB = (_shiftRegState & tempBitmask) ? (PORTB | SHIFT_DATA_PIN) : (PORTB & I_SHIFT_DATA_PIN);

        //pulse the clock for shift
        PORTB |= SHIFT_CLOCK_PIN;
        PORTB &= I_SHIFT_CLOCK_PIN;
        tempBitmask <<= 1;

    }

    PORTB |= SHIFT_LATCH_PIN;
}

void NeuroBoard::writeLEDs(byte outByte) {

    PORTB &= I_SHIFT_LATCH_PIN;
    _shiftRegState = outByte;
    byte tempBitmask = BITMASK_ONE;

    for (int i = 0; i < MAX_LEDS; i++) {

        PORTB = (outByte & tempBitmask) ? (PORTB | SHIFT_DATA_PIN) : (PORTB & I_SHIFT_DATA_PIN);

        //pulse the clock for shift
        PORTB |= SHIFT_CLOCK_PIN;
        PORTB &= I_SHIFT_CLOCK_PIN;
        tempBitmask <<= 1;

    }

    PORTB |= SHIFT_LATCH_PIN;
}

void NeuroBoard::writeLED(const int& led, const bool& state) {

    const int LED = this->ledPins[led];

    byte bitMask = BITMASK_ONE;

    #ifdef ARDUINO_AVR_UNO
        if (!(8 < LED < 13)) return;
    #else // We must be dealing with a Leonardo
        if (!(0 < LED < 7)) return;
    #endif

    if (state) {
        _shiftRegState |= bitMask << (7 - LED);
    } else {
        _shiftRegState &= ~(bitMask << (7 - LED));
    }

    writeLEDs();

}

/* ******************************************************* */
