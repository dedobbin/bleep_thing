app:	bleep_thing.o sound_player.o
	g++ -o bleep_thing -pthread -lwiringPi bleep_thing.o sound_player.o

bleep_thing.o:	bleep_thing.cpp
	g++ -c bleep_thing.cpp

sound_player.o: sound_player.cpp
	g++ -c sound_player.cpp




