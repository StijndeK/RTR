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

	enum modulationType
	{
		Amp,
		Pitch
	};

	void setGain(float gain);
	void setPan(float p);
	//void setGainEnvelopes(float attack, float range, float curve);
	void setEnvelopes(modulationType type, float attack, float range, float curve);

	void setAttack(float attack);
	void setOffset(float offset);

	string getAudioName(FMOD_SOUND* sound);
	Layer* getLayerByName(string name);

	bool trigger = 0; // triggers envelopes
	float _gain = 1;
	float gainSnapshot = 1;

	// vectors holding collections of layers
	vector<Layer*> layerImpacts;
	vector<Layer*> layerLoops;
	vector<Layer*> pitchModLayers;

private:
	Envelopes attackEnv;

	float frequencyStandard = 44100;
};
