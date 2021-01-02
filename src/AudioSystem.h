#pragma once
#include "ofMain.h"
#include "fmod.h"
#include "Layer.h"

class AudioSystem {

public:
	AudioSystem();
	~AudioSystem();

	void initFMODSystem();
	void update();
	void loadAudio();
	FMOD_SOUND* initSound(FMOD_MODE mode, ofDirectory& directory, int& numb);
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

	// vector of layers
	std::vector<Layer*> layerImpacts;
	Layer* layerSub;
	Layer* layerTest;
};
