#pragma once

#include "ofMain.h"
#include "fmod.h"

class AudioSystem {

public:
	void initFMODSystem();
	void update();
	void loadAudio();
	void playAudio();

	// specific functions to the engine or framework
	// TOD: create sperate class for these functions
	static void debugMessage(string message);
	const char* getAudioPath(string filename);
	static void getDriverInfo();


	FMOD_SOUND* sound;
	FMOD_CHANNEL* channel;
};
