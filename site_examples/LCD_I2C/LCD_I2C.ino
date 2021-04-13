/**
 * LCD Example.
 * 
 * Ported By: Ben Antonellis
 * Date: February 18th, 2021
**/

#include <Wire.h>
#include "LiquidCrystal_I2C.h"
//#include "NeuroBoard.hpp"

LiquidCrystal_I2C lcd(0x27, 16, 2); 				// Set the LCD address to 0x27 for a 16 chars and 2 line display
#define MAX 800 									// Maximum possible reading. TWEAK THIS VALUE!!

int finalReading;
int Lit_Bank;
//NeuroBoard board;

void setup(){
	Serial.begin(9600); 							// Begin serial communications
	lcd.init(); 									// Initialize the lcd
	lcd.backlight();
}

void loop(){

	finalReading = analogRead(A0); 			// Reads in the Amplified EMG
	delay(10); 										// 10 ms delay
	lcd.clear(); 									// initialize the lcd

	Serial.print(finalReading);
	Serial.print("\t");
	finalReading = constrain(finalReading, 0, MAX); 
	Lit_Bank = map(finalReading, 0, MAX, 0, 16); 	// Determines length of bar to light up depending on your EMG level
	Serial.println(Lit_Bank);

	if (Lit_Bank == 0) {
        lcd.setCursor(0,0);
        lcd.print("");
        lcd.setCursor(0,1);
        lcd.print("");
    }

	if (Lit_Bank == 1) {
        lcd.setCursor(0,0);
        lcd.print("0");
        lcd.setCursor(0,1);
        lcd.print("0");
    }

	if (Lit_Bank == 2) {
        lcd.setCursor(0,0);
        lcd.print("00");
        lcd.setCursor(0,1);
        lcd.print("00");
    }

	if (Lit_Bank == 3) {
        lcd.setCursor(0,0);
        lcd.print("000");
        lcd.setCursor(0,1);
        lcd.print("000");
    }
         
	if (Lit_Bank == 4) {
        lcd.setCursor(0,0);
        lcd.print("0000");
        lcd.setCursor(0,1);
        lcd.print("0000");
    }
         
	if (Lit_Bank == 5) {
        lcd.setCursor(0,0);
        lcd.print("00000");
        lcd.setCursor(0,1);
        lcd.print("00000");
    }
         
	if (Lit_Bank == 6) {
        lcd.setCursor(0,0);
        lcd.print("000000");
        lcd.setCursor(0,1);
        lcd.print("000000");
    }
         
	if (Lit_Bank == 7) {
        lcd.setCursor(0,0);
        lcd.print("0000000");
        lcd.setCursor(0,1);
        lcd.print("0000000");
    }
         
	if (Lit_Bank == 8) {
        lcd.setCursor(0,0);
        lcd.print("00000000");
        lcd.setCursor(0,1);
        lcd.print("00000000");
    }
         
	if (Lit_Bank == 9) {
        lcd.setCursor(0,0);
        lcd.print("000000000");
        lcd.setCursor(0,1);
        lcd.print("000000000");
    }
         
	if (Lit_Bank == 10) {
        lcd.setCursor(0,0);
        lcd.print("0000000000");
        lcd.setCursor(0,1);
        lcd.print("0000000000");
    }
         
	if (Lit_Bank == 11) {
        lcd.setCursor(0,0);
        lcd.print("00000000000");
        lcd.setCursor(0,1);
        lcd.print("00000000000");
    }
         
	if (Lit_Bank == 12) {
        lcd.setCursor(0,0);
        lcd.print("000000000000");
        lcd.setCursor(0,1);
        lcd.print("000000000000");
    }
         
	if (Lit_Bank == 13) {
        lcd.setCursor(0,0);
        lcd.print("0000000000000");
        lcd.setCursor(0,1);
        lcd.print("0000000000000");
    }
         
	if (Lit_Bank == 14) {
        lcd.setCursor(0,0);
        lcd.print("00000000000000");
        lcd.setCursor(0,1);
        lcd.print("00000000000000");
    }
         
	if (Lit_Bank == 15) {
        lcd.setCursor(0,0);
        lcd.print("000000000000000");
        lcd.setCursor(0,1);
        lcd.print("000000000000000");
    }
         
	if (Lit_Bank == 16) {
        lcd.setCursor(0,0);
        lcd.print("0000000000000000");
        lcd.setCursor(0,1);
        lcd.print("0000000000000000");
    }

}
