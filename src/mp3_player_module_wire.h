/*
 * mp3_player_module_wire.h - OneWire driver for mp3 player modules.

 * Track file names must be 5 digits starting from 00001.mp3 00002.mp3 00003.mp3...
 * Also applies for directory names.
 * 
 * https://github.com/arduino12/mp3_player_module_wire
 */
#ifndef _MP3_PLAYER_MODULE_WIRE_H_
#define _MP3_PLAYER_MODULE_WIRE_H_

#include <Arduino.h>

class Mp3PlayerModuleWire
{
public:
	enum {
		_WIRE_HIGH_US = 1000,				// DY_SV17F also works with 100us but JQ8400 needs 1ms
		_WIRE_LOW_US = 4000,
		_WIRE_SHORT_US = 400,
		_WIRE_LONG_US = 1200,
		_CMD_DELAY_MS = 80,
	};
	enum {
		MIN_VOLUME = 0,
		MAX_VOLUME = 30,					// (default is 20)
		EQ_NORMAL = 0,						// (default)
		EQ_POP = 1,
		EQ_ROCK = 2,
		EQ_JAZZ = 3,
		EQ_CLASSIC = 4,
		PLAY_ALL_REPEAT = 0,				// play all tracks non-stop.
		PLAY_TRACK_REPEAT = 1,				// play current track non-stop.
		PLAY_TRACK = 2,						// play current track and stop (default).
		PLAY_RANDOM_TRACKS = 3,				// play random tracks non-stop.
		PLAY_DIR_REPEAT = 4,				// play all tracks in directory non-stop.
		PLAY_DIR_RANDOM_TRACKS = 5,			// play random tracks in directory non-stop.
		PLAY_DIR_SEQUENCE = 6,				// play all tracks in directory and stop.
		PLAY_ALL_SEQUENCE = 7,				// play all tracks and stop.
		STORAGE_SD = 0,
		STORAGE_USB_DISK = 1,
		STORAGE_FLASH = 2,					// (default)
	};

	Mp3PlayerModuleWire() { }
	Mp3PlayerModuleWire(uint8_t pin) { begin(pin); }

	void begin(uint8_t pin);
	void play();
	void pause();
	void stop();
	void prev();
	void next();
	void next_dir();
	void prev_dir(bool clear_track_index=true);
	void set_track_index(uint16_t track_id);// track name must be 5 digits like 00105.mp3
	void set_volume(uint8_t volume);		// 1-30
	void set_eq(uint8_t eq);				// 0-3, see EQ_* enums
	void set_play_mode(uint8_t mode);		// 0-7, see PLAY_* enums
	void set_channel(uint8_t ch);			// 0=work 1=mute
	void set_storage(uint8_t storage);		// 0-2, see STORAGE_* enums (I tried sending the dir index like the doc suggests but the module ignored..)
	void sleep();							// JQ8400 supports it! drop current consumption to 140uA if amplifier is disabled!
	void stop2();							// stops playing like stop()
// private:
	void _write_byte(uint8_t data);
	void _send_cmd(uint8_t cmd);
	void _write_num(uint16_t num);
	uint8_t _pin;							// TODO: replace with raw GPIO register writes
};

#endif