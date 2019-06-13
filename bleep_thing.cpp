#include "sound_player.h"
#include <stdlib.h>
#include <unistd.h>
#include <ctime>
#include <wiringPi.h>
#include <thread>
#include <iostream>
#include <string>

#define BUTTON_PIN 1 
#define MAX_THREADS 10 

//Should refactor ugly globals maybe
bool seeded = false;
bool locked = false;
int currentStressIntensity = 0;


void setStress(bool on = true, int intensity = 8){
    if (on && intensity != currentStressIntensity){
      std:: cout << "stress on: " << intensity << std::endl;
      std::string cmd = "sysbench --test=cpu --num-threads=" + std::to_string(intensity) + " --cpu-max-prime=50000 run > /dev/null 2>&1 &";
      system(cmd.c_str());
      currentStressIntensity = intensity;
    } else if (!on || intensity == 0){
      std::cout << "turn stress off" << std::endl;
      system("pkill sysbench");
      currentStressIntensity = 0;
    }
}

float randomFloat(float min, float max){
    if (!seeded)
      srand(time(0));
    float value = (float)rand() / (float)(RAND_MAX/max-min) + min;
    return value;
}

int randomInt(int min, int max){
    if (!seeded)
        srand(time(0));
    int value = rand() / (RAND_MAX/max-min) + min;
    return value;
}

void generateValues (int* minFreq, int* maxFreq, float* minDuration, float* maxDuration){
    if (maxFreq){
        *maxFreq = randomInt(4001, 40000);
        std::cout << "maxFreq = " << *maxFreq;
    }
    if (minFreq){
        *minFreq = randomInt(4000, *maxFreq);
	std::cout << " minFreq = " << *minFreq;
    }
    if (minDuration){
        *minDuration = randomFloat(0.0, 5.0);
        std::cout << " minDuration = " << *minDuration;
    }
    if (maxDuration){
        *maxDuration = randomFloat(*minDuration, 2);
        std::cout << " maxDuration = "<< *maxDuration;
    }
    std::cout << std::endl;
}

void readButton(int* minFreq, int* maxFreq, float* minDuration, float* maxDuration, bool *stressCpu){
    while(true){
        int state = digitalRead(BUTTON_PIN);
        if (state == 0){
	    locked = true;
	    setStress(*stressCpu);
	    *stressCpu = !(*stressCpu);
	    generateValues(minFreq, maxFreq, minDuration, maxDuration);
	    locked = false;
	    sleep(1);
	}
    }
}

int main (int argc, char* argv[]){
    int numTones;
    int minFreq;
    int maxFreq;
    
    float minDuration;
    float maxDuration;
   
    bool stressCpu = true;  
   
    generateValues(&minFreq, &maxFreq, &minDuration, &maxDuration);

    wiringPiSetup(); 
    pinMode(BUTTON_PIN, INPUT);
    pullUpDnControl(BUTTON_PIN, PUD_UP);

    SoundPlayer p = SoundPlayer(0);
    std::thread th(readButton, &minFreq, &maxFreq, &minDuration, &maxDuration, &stressCpu);
    while (true){
	if (!locked){
            int pack[3] = {randomInt(minFreq, maxFreq)/2, randomInt(minFreq, maxFreq)*2, randomInt(minFreq, maxFreq)}; 
	    p.play(pack, 3, randomFloat(minDuration, maxDuration), true);
	}
    } 
}

