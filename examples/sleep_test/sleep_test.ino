/*
 * sleep_test.ino - put mp3 player to sleep to save power,
 * wake up and play a single track from flash in 5 second interval.
 * Must connect the module via USB and add a track named 00003.mp3.
 *
 * JQ8400: dropping from 18mA to 4.5mA,
 * the sleep command only work on the one-wire protocol, via UART (Serial) it does nothing,
 * After removing the BUSY-LED and modifing the PCB so that the SD (shutdown) pin of
 * the HXJ8002 3W audio amplifier goes to the MCU (instead of GND)- the sleep current drops to 140uA!
 *
 * Connect IO4 (ONE_LINE) pin to arduino pin D4 (can be any pin).
 * Connect player audio amplifier SD (shutdown) pin (must lift it because PCB shorts it to GND) to D6 (can be any pin).
 * 
 * Track file names must be 5 digits starting from 00001.mp3 00002.mp3 00003.mp3...
 * Also applies for directory names.
 * 
 * https://github.com/arduino12/mp3_player_module_wire
 */
#include <Arduino.h>
#include "mp3_player_module_wire.h"

#define PLAYER_WIRE_PIN					(4)		// connect to ONE_LINE
#define PLAYER_SD_PIN					(6)		// connect to audio amplifier SD (shutdown) pin (must lift it because PCB shorts it to GND)

Mp3PlayerModuleWire player(PLAYER_WIRE_PIN);

void set_player_power(bool is_on)
{
	digitalWrite(PLAYER_SD_PIN, !is_on);
	if (!is_on)
		player.sleep();
}

void setup()
{
	Serial.begin(115200);
	pinMode(PLAYER_SD_PIN, OUTPUT);
	set_player_power(false);
}

void loop()
{
	Serial.println(F("Wake up player and start playing track 00003.mp3..."));
	set_player_power(true);
	player.set_track_index(3);
	player.play();
    delay(2000);
	Serial.println(F("Put player to sleep to save power (wake up in 5 seconds)..."));
	set_player_power(false);
    delay(5000);
}
