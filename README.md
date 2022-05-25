# mp3_player_module_wire
A simple library to interface with mp3 player modules ([DY_SV17F](https://attach01.oss-us-west-1.aliyuncs.com/IC/Datasheet/13288.pdf), [JQ8400](https://sparks.gogo.co.nz/assets/_site_/downloads/JQ8400_English.pdf)) using 1 wire.

## Features
* Low current consumption sleep mode for JQ8600 - **140uA!**
* Basic player commands (`play`, `pause`, `set_volume`, `set_track_index`...).
* Internally fix weird behaviour of `prev_dir` and `set_play_mode`.
* Requires only one GPIO pin (used only as OUTPUT) - can be any pin.

## Alternatives
If you need more control over DY_SV17F you better use [dyplayer](https://github.com/SnijderC/dyplayer) library by @SnijderC,  
It gives abstracted UART Control of DY-XXXX mp3 modules.  
If you need more control over JQ8400 you better use [JQ8400_Serial](https://github.com/sleemanj/JQ8400_Serial) library by @sleemanj.

## Examples
I made a [basic_test](https://github.com/arduino12/dy_sv17f_wire/blob/main/examples/basic_test/basic_test.ino), a 
[busy_pin_test](https://github.com/arduino12/dy_sv17f_wire/blob/main/examples/busy_pin_test/busy_pin_test.ino), a
[power_switching_test](https://github.com/arduino12/dy_sv17f_wire/blob/main/examples/power_switching_test/power_switching_test.ino) (for DY_SV17F) and a [sleep_test](https://github.com/arduino12/dy_sv17f_wire/blob/main/examples/sleep_test/sleep_test.ino) (for JQ8400).

## Low Current Consumption
The only reason I made this code is that I wanted a battery-powered (low idle current) player-  
And I saw a "sleep" command on those chinese playere "datasheets" - but only for the one-wire protocol...  
As it turned out- the command is useless on DY_SV17F and no current reduction whatsoever...  
(the idle current is 14mA including the LTK5128 5W audio amplifier with its SD tied to GND and the 32Mbit flash chip)  
So using DY_SV17F- the best low current approach would be to switch it via a N-channel MOSFET - with the cost of extra time (400ms+) it takes to boot- see above [power_switching_test](https://github.com/arduino12/dy_sv17f_wire/blob/main/examples/power_switching_test/power_switching_test.ino) example.

On the JQ8400 it was a success! dropping from 18mA to 4.5mA,  
After removing the BUSY-LED and modifing the PCB so that the  
SD (shutdown) pin of the HXJ8002 3W audio amplifier goes to the MCU (instead of GND)-  
I was able to get a sleep current of 140uA! see above [sleep_test](https://github.com/arduino12/dy_sv17f_wire/blob/main/examples/sleep_test/sleep_test.ino) example.  
This sleep command only work on the one-wire protocol, when sent via UART (Serial) it does nothing.

## Enjoy!
A.E.TECH
