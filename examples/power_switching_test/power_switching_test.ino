/*
 * power_switching_test.ino - turn on mp3 player module using N-channel MOSFET,
 * and play a single track from flash in 5 second interval.
 * Must connect the module via USB and add a track named 00003.mp3.
 * 
 * DY_SV17F: Must connect CON1 and CON2 to GND, and CON3 to VCC via 10K resistors-
 * So the module will enter UART+OneLine mode (AKA OneWire)-
 * Those pins must be connected BEFORE powering the module - they are internally sampled at powerup!
 * 
 * Connect IO4 (ONE_LINE) pin to arduino pin D4 (can be any pin).
 * Connect player GND to drain, arduino GND to source and arduino D6 to gate via 1K resistor (can be any pin).
 * 
 * Track file names must be 5 digits starting from 00001.mp3 00002.mp3 00003.mp3...
 * Also applies for directory names.
 * 
 * https://github.com/arduino12/mp3_player_module_wire
 */
#include <Arduino.h>
#include "mp3_player_module_wire.h"

#define PLAYER_WIRE_PIN					(4)		// connect to ONE_LINE
#define PLAYER_FET_PIN					(6)		// connect to N-FET gate via 1K resistor
#define PLAYER_BOOT_MS					(400)	// may increase with the track count on the module (JQ8400 needs 500ms)

Mp3PlayerModuleWire player(PLAYER_WIRE_PIN);

void set_player_power(bool is_on)
{
	digitalWrite(PLAYER_FET_PIN, is_on);
	if (is_on)
		delay(PLAYER_BOOT_MS);
}

void setup()
{
	Serial.begin(115200);
	pinMode(PLAYER_FET_PIN, OUTPUT);
	set_player_power(false);
}

void loop()
{
	Serial.println(F("Turn player ON and start playing track 00003.mp3..."));
	set_player_power(true);
	player.set_track_index(3);
	player.play();
    delay(2000);
	Serial.println(F("Turn player OFF to save power (wake up in 5 seconds)..."));
	set_player_power(false);
    delay(5000);
}
