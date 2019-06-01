#include "sound_player.h"
#include <stdlib.h> 
#include <iostream>
#include <string>

bool fileExists(std::string path){
    if (FILE *file = fopen(path.c_str(), "r")) {
        fclose(file);
        return true;
    } 
    return false;
}

void execute(std::string cmd){
    system(cmd.c_str());
}

SoundPlayer::SoundPlayer(int _device)
:device(_device) {
}

void SoundPlayer::play(int tone, float duration, bool blocking){
    std::string cmd = "AUDIODEV=hw:" + std::to_string(device) + " play -q -n synth " + std::to_string(duration) + " sin "  + std::to_string(tone) + " 2> /dev/null"; 
    if (blocking)
        execute(cmd);
    else{ 
        std::thread t(execute, cmd);
        t.detach();
        //threads.push_back(std::thread(execute, cmd));
    }
}


void SoundPlayer::play(int* tones, int numTones, float duration, bool blocking){
    std::string s = " ";
    std::string cmd = "";
    for (int i = 0; i < numTones; i ++){
        s += "sin " + std::to_string(tones[i]) + " ";
    }
    cmd = "AUDIODEV=hw:" + std::to_string(device) + " play -q -n synth " + std::to_string(duration) + s + " 2> /dev/null"; 
    if (blocking)
        execute(cmd);
    else 
        threads.push_back(std::thread(execute, cmd));
}

void SoundPlayer::join(){
    for (auto& th : threads)
        th.join();
}

