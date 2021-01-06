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
	void stopAudio();

	string getAudioName(FMOD_SOUND* sound);

	void setGain(float gain);
	void setPan(float p);
	void setEnvelope(float attack);
	void setAttack(float attack);
	void setOffset(float offset);

	bool trigger = 0; // triggers envelopes

	float _gain = 1;
	float gainSnapshot = 1;

	vector<Layer*> layerSelection;
private:
	// FMOD_CHANNEL* channel;
	FMOD_CHANNEL* channelImpacts;
	FMOD_CHANNEL* channelLoops;
	FMOD_SOUNDGROUP* soundGroup;

	// vector of layers
	vector<Layer*> layerImpacts;
	vector<Layer*> layerLoops;
	//vector<Layer*> layerImpacts;
	//vector<Layer*> layerSubs;
	//vector<Layer*> layerStartPads;
	//vector<Layer*> layerEndPads;
	//vector<Layer*> layerFx;
	//vector<Layer*> layerNoises;
	//vector<Layer*> layerShepards;

	Envelopes rangeEnv;
	Envelopes attackEnv;
};
