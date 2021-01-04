#pragma once
#include "ofMain.h"
#include "fmod.h"
#include "Layer.h"
#include "AudioBase.h"

class AudioSystem : public AudioBase {

public:
	AudioSystem();
	~AudioSystem();

	void initFMODSystem();
	void update();
	void loadAudio();
	void playAudio();
	void stopAudio();
	string getAudioName(FMOD_SOUND* sound);
	void setGain(float gain);
	void setPan(float p);

private:
	std::vector<FMOD_SOUND*> sounds;
	FMOD_CHANNEL* channel;

	// vector of layers
	std::vector<Layer*> layerImpacts;
	std::vector<Layer*> layerSubs;
	std::vector<Layer*> layerTests;
};
