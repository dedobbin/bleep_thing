#ifndef SOUND_PLAYER
#define SOUND_PLAYER

/**
*   Very simple wrapper for SoX
*   For the Raspberry Pi the following installation are needed:
*   sudo apt-get install sox
*   sudo apt-get install mp3
*   Compile using g++ -pthread
*   In the future should bypass the system() stuff, but for now this works.
**/

#include <string>
#include <vector>
#include <thread>


enum SoundDevice{
    STANDARD,
    USB
};

class SoundPlayer{
    public:
        SoundPlayer(int device = SoundDevice::STANDARD);
        //Play single tone
        void play(int tone, float duration, bool blocking);
        //Play multiple tones
        void play(int* tones, int numTones, float duration, bool blocking = false);  
        void join();
        
        const int device;
    
    private:
        int volume;
        std::vector<std::thread> threads;
        
};

#endif
