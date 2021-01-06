#pragma once
#include "ofMain.h"
#include "fmod.h"
#include "Layer.h"
#include "AudioBase.h"
#include "Envelopes.h"

class AudioSystem : public AudioBase {

public:
	AudioSystem();
	~AudioSystem();

	void initFMODSystem();
	void update();
	void loadAudio();
	void playAudioLoops();
	void playAudioImpacts();
	void stopAudio(vector<Layer*> layersToStop);

	string getAudioName(FMOD_SOUND* sound);

	void setGain(float gain);
	void setPan(float p);
	void setEnvelope(float attack);
	void setAttack(float attack);
	void setOffset(float offset);

	Layer* getLayerByName(string name);

	bool trigger = 0; // triggers envelopes

	float _gain = 1;
	float gainSnapshot = 1;

	// vector of layers
	vector<Layer*> layerImpacts;
	vector<Layer*> layerLoops;

private:

	Envelopes rangeEnv;
	Envelopes attackEnv;
};
