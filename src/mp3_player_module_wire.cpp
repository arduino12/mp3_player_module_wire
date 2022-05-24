/*
 * mp3_player_module_wire.cpp - OneWire driver for mp3 player modules.

 * Track file names must be 5 digits starting from 00001.mp3 00002.mp3 00003.mp3...
 * Also applies for directory names.
 * 
 * https://github.com/arduino12/mp3_player_module_wire
 */
#include "mp3_player_module_wire.h"

void Mp3PlayerModuleWire::begin(uint8_t pin)
{
	_pin = pin;
	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, HIGH);
}

void Mp3PlayerModuleWire::_write_byte(uint8_t data)
{
	noInterrupts();

	delayMicroseconds(_WIRE_HIGH_US);
	digitalWrite(_pin, LOW);
	delayMicroseconds(_WIRE_LOW_US);
	digitalWrite(_pin, HIGH);

	for (uint8_t i = 0; i < 8; i++, data >>= 1) {
		if (data & 1) {
			delayMicroseconds(_WIRE_LONG_US);
			digitalWrite(_pin, LOW);
			delayMicroseconds(_WIRE_SHORT_US);
		}
		else {
			delayMicroseconds(_WIRE_SHORT_US);
			digitalWrite(_pin, LOW);
			delayMicroseconds(_WIRE_LONG_US);
		}
		digitalWrite(_pin, HIGH);
	}

	interrupts();
}

void Mp3PlayerModuleWire::_send_cmd(uint8_t cmd)
{
	_write_byte(cmd);
	delay(_CMD_DELAY_MS);
}

void Mp3PlayerModuleWire::_write_num(uint16_t num)
{
	for (uint16_t i = 10000, n = num; i > 0; i /= 10)
		if (num >= i) {
			_write_byte(n / i);
			n %= i;
		}
}

// 1-30
void Mp3PlayerModuleWire::set_volume(uint8_t volume)
{
	_write_num(volume);
	_send_cmd(0x0c);
}

// 0-3, see EQ_* enums in dy_sv17f_wire.h
void Mp3PlayerModuleWire::set_eq(uint8_t eq)
{
	_write_byte(eq);
	_send_cmd(0x0d);
}

// 0-7, see PLAY_* enums in dy_sv17f_wire.h
void Mp3PlayerModuleWire::set_play_mode(uint8_t mode)
{
	if (mode >= PLAY_DIR_SEQUENCE) { // restart internal sequence
		set_play_mode(PLAY_TRACK);
	}
	_write_byte(mode);
	_send_cmd(0x0e);
}

// 0=work 1=mute
void Mp3PlayerModuleWire::set_channel(uint8_t ch)
{
	_write_byte(ch);
	_send_cmd(0x0f);
}

// track name must be 5 digits like 00105.mp3
void Mp3PlayerModuleWire::set_track_index(uint16_t track_id)
{
	_write_num(track_id);
	_write_byte(0x0b);
	_send_cmd(0x10);
}

void Mp3PlayerModuleWire::play()
{
	_send_cmd(0x11);
}

void Mp3PlayerModuleWire::pause()
{
	_send_cmd(0x12);
}

void Mp3PlayerModuleWire::stop()
{
	_send_cmd(0x13);
}

void Mp3PlayerModuleWire::prev()
{
	_send_cmd(0x14);
}

void Mp3PlayerModuleWire::next()
{
	_send_cmd(0x15);
}

void Mp3PlayerModuleWire::prev_dir(bool clear_track_index)
{
	_send_cmd(0x16);
	if (clear_track_index) { // this is the only way I found setting the track index to 0 like the next_dir() does.
		next_dir();
		prev_dir(false);
		prev_dir(false);
		next_dir();
	}
}

void Mp3PlayerModuleWire::next_dir()
{
	_send_cmd(0x17);
}

// 0-2, see STORAGE_* enums (I tried sending the dir index like the doc suggests but the module ignored..)
void Mp3PlayerModuleWire::set_storage(uint8_t storage)
{
	_send_cmd(0x18 + storage);
}

// JQ8400 supports it! drop current consumption to 140uA if amplifier is disabled!
void Mp3PlayerModuleWire::sleep()
{
	_send_cmd(0x1b);
}

// stops playing like stop()
void Mp3PlayerModuleWire::stop2()
{
	_send_cmd(0x1c);
}
