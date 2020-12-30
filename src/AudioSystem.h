#pragma once
#include "ofMain.h"
#include "fmod.h"

class AudioSystem {

public:
	AudioSystem();
	~AudioSystem();

	void initFMODSystem();
	void update();
	void loadAudio();
	void playAudio();
	void stopAudio();
	void setGain(float gain);
	void setPan(float p);

	// specific functions to the engine or framework
	// TOD: create sperate class for these functions
	static void debugMessage(string message);
	const char* getAudioPath(string filename);
	static void getDriverInfo();

	std::vector<FMOD_SOUND*> sounds;
	FMOD_CHANNEL* channel;
};
