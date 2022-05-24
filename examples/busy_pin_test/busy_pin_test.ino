/*
 * busy_pin_test.ino - set mp3 player module to play tracks from flash using 1 wire for commands
 * and another BUSY wire so MCU can read and know if the track has ended.
 * 
 * DY_SV17F: Must connect CON1 and CON2 to GND, and CON3 to VCC via 10K resistors-
 * So the module will enter UART+OneLine mode (AKA OneWire)-
 * Those pins must be connected BEFORE powering the module - they are internally sampled at powerup!
 * 
 * Connect IO4 (ONE_LINE) pin to arduino pin D4 (can be any pin).
 * Connect CON3 (BUSY) pin to arduino pin D5 (can be any pin).
 * 
 * Track file names must be 5 digits starting from 00001.mp3 00002.mp3 00003.mp3...
 * Also applies for directory names.
 * 
 * https://github.com/arduino12/mp3_player_module_wire
 */
#include <Arduino.h>
#include "mp3_player_module_wire.h"

#define PLAYER_WIRE_PIN					(4)
#define PLAYER_BUSY_PIN					(5)

Mp3PlayerModuleWire player(PLAYER_WIRE_PIN);

void wait_for_end_of_track()
{
#ifdef PLAYER_BUSY_PIN
	delay(600);					// may need to increase if using large track files or other storage
	for (uint16_t debounce = 0; debounce != 0xff00;) {
		debounce = (debounce << 1) | digitalRead(PLAYER_BUSY_PIN) | 0xfe00;
		delay(10);
	}
	delay(300);					// may need to increase if using large track files or other storage
#else
	delay(2000);                // no way of knowing when the track ends
#endif
}

void setup()
{
	Serial.begin(115200);
	Serial.println(F("Setting default values"));

	player.set_volume(20);						// default
	player.set_eq(player.EQ_NORMAL);			// default
	player.set_play_mode(player.PLAY_TRACK);	// default
	player.set_storage(player.STORAGE_FLASH);	// default

	Serial.println(F("Playing with equalizer..."));
	for (int8_t i = player.EQ_CLASSIC; i >= 0; i--) {
		player.set_eq(i);
		player.play();
		wait_for_end_of_track();
	}

	Serial.println(F("Playing a specific track once..."));
	player.set_play_mode(player.PLAY_TRACK);
	player.set_track_index(3); // 00003.mp3
	player.play();

	Serial.println(F("Playing and stopping quick..."));
	player.next();
	for (uint8_t i = 0; i < 5; i++) {
		player.play();
		delay(700 + i * 100);
		player.stop();
		delay(1000);
	}

	Serial.println(F("Playing with pausing in the middle..."));
	player.next();
	for (uint8_t i = 0; i < 5; i++) {
		player.play();
		delay(700 + i * 100);
		player.pause();
		delay(700);
		player.play();
		wait_for_end_of_track();
	}

	player.next_dir();

	Serial.println(F("Playing random tracks inside first directory..."));
	player.set_play_mode(player.PLAY_DIR_RANDOM_TRACKS);
	player.play();
	wait_for_end_of_track();

	Serial.println(F("Playing random tracks inside second directory..."));
	player.next_dir();
	player.play();
	wait_for_end_of_track();

	Serial.println(F("Playing same track in repeat for 10 seconds..."));
	player.set_play_mode(player.PLAY_TRACK_REPEAT);
	player.play();
	delay(10000);

	Serial.println(F("Playing all tracks in repeat..."));
	player.set_play_mode(player.PLAY_ALL_REPEAT);
	player.play();
}

void loop()
{

}
