/*
 * basic_test.ino - set mp3 player module to play a single track from flash using 1 wire,
 * Must connect the module via USB and add a track named 00003.mp3.
 * 
 * DY_SV17F: Must connect CON1 and CON2 to GND, and CON3 to VCC via 10K resistors-
 * So the module will enter UART+OneLine mode (AKA OneWire)-
 * Those pins must be connected BEFORE powering the module - they are internally sampled at powerup!
 * 
 * Connect IO4 (ONE_LINE) pin to arduino pin D4 (can be any pin).
 * 
 * Track file names must be 5 digits starting from 00001.mp3 00002.mp3 00003.mp3...
 * Also applies for directory names.
 * 
 * https://github.com/arduino12/mp3_player_module_wire
 */
#include <Arduino.h>
#include "mp3_player_module_wire.h"

#define PLAYER_WIRE_PIN					(4)

Mp3PlayerModuleWire player(PLAYER_WIRE_PIN);

void setup()
{
	Serial.begin(115200);

	player.set_volume(20);
	player.set_track_index(3);
}

void loop()
{
	Serial.println(F("Start playing track 00003.mp3 once at 20/30 volume level..."));
	player.play();
    delay(5000);
}
