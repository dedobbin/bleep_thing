# bleep_thing
I wanted to see if i could make 'tremolo' effect by playing tones on a Raspberry Pi, then put a load on CPU so it would cut out the sound output periodically. 

The answer is, sorta.

## Control
Use button connected to pin 1 to change random tones being played.   
Currently working on a 'module' to control CPU load, using potmeter and MCP3008.

## Build
Just run `make`.  
Needs [wiringPi](http://wiringpi.com/) library to build. 
