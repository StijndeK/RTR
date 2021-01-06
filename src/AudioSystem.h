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
	void playAudio();
	void stopAudio();

	string getAudioName(FMOD_SOUND* sound);

	void setGain(float gain);
	void setPan(float p);
	void setEnvelope(float attack);
	void setAttack(float attack);

	bool trigger = 0; // triggers envelopes

	float _gain = 1;
	float gainSnapshot = 1;
private:
	FMOD_CHANNEL* channel;

	// vector of layers
	vector<vector<Layer*>> layerOneshots;
	vector<Layer*> layerImpacts;
	vector<Layer*> layerSubs;
	vector<vector<Layer*>> layerLoops;
	vector<Layer*> layerStartPads;
	vector<Layer*> layerEndPads;
	vector<Layer*> layerFx;
	vector<Layer*> layerNoises;
	vector<Layer*> layerShepards;

	float testFq = 0.0;
	Envelopes rangeEnv;
	Envelopes attackEnv;
};
