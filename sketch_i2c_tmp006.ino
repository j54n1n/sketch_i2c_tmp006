// An example to get started with the TMP006 temperature sensor
// Modified by SparkFun Electronics. 
// Modified by Julian Sanin.
// We release this code under ([Beerware license](http://en.wikipedia.org/wiki/Beerware)).

//Original Code license:
/***************************************************
This is a library for the TMP006 Temp Sensor

Designed specifically to work with the Adafruit TMP006 Breakout
----> https://www.adafruit.com/products/1296

These displays use I2C to communicate, 2 pins are required to
interface
Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.
BSD license, all text above must be included in any redistribution
****************************************************/

/* Wiring Diagram
 * ------------------.                   .-----------------
 *                   |                   |
 *                   [A5/SCL]xxxxxxxx[SCL]
 *    Arduino Uno    [A4/SDA]xxxxxxxx[SDA]  TMP006 Breakout
 *  (or compatible)  |                   |       Board
 *                   [5V]xxxxxxxxxxxx[VCC]
 *                   [GND]xxxxxxxxxxx[GND]
 *                   |                   |
 * ------------------�                   �-----------------
 */

// Includes:
#include "Tmp006.h"
#include <Wire.h>

// Configurations:
enum {
	SERIAL_SPEED = 9600,   // Baud rate
	TMP006_ADDRESS = 0x40, // I2C Address
	TMP006_SAMPLES = 1,    // # of samples [1|2|4|8|16]
};

enum PrintOptions {
	PRINT_PLOT,
	PRINT_TEXT
};

// Prototypes:
void printTmp006(float objTemp, float senTemp, PrintOptions opts);

// Global variables:
uint8_t tmp006Sensor1 = TMP006_ADDRESS;
uint16_t tmp006Sensor1Cfg = Tmp006_sensorCfg(TMP006_SAMPLES);

void setup() {
	Serial.begin(SERIAL_SPEED);
	Tmp006_config(tmp006Sensor1, tmp006Sensor1Cfg);
}

void loop() {
	float objTemp = Tmp006_readObjTempC(tmp006Sensor1);
	float senTemp = Tmp006_readDieTempC(tmp006Sensor1);
	printTmp006(objTemp, senTemp, PrintOptions::PRINT_TEXT);
	delay(Tmp006_sensorDelay(TMP006_SAMPLES));
}

void printTmp006(float objTemp, float senTemp, PrintOptions opts) {
	if (opts == PRINT_PLOT) {
		Serial.print(objTemp);
		Serial.print('\t');
		Serial.println(senTemp);
		return;
	}
	Serial.print(F("Object Temperature: ")); 
	Serial.print(objTemp);
	Serial.print(F("*C\t"));
	Serial.print(F("Sensor Temperature: ")); 
	Serial.print(senTemp);
	Serial.println(F("*C"));
}
