#pragma once
#include "ofMain.h"
#include "fmod.h"
#include "Layer.h"
#include "AudioBase.h"
#include "Envelopes.h"
#include "ModulationData.h"

class AudioSystem : public AudioBase, public ModulationTypes {

public:
	AudioSystem();
	~AudioSystem();

	enum modulationType{ Amp, Pitch};

	void initFMODSystem();
	void update();
	void loadAudio();
	void playAudioLoops();
	void playAudioImpacts();
	void playEnvelopes();
	void stopAudio(vector<Layer*> layersToStop);

	void setGain(float gain);
	void setPan(float p);
	void setEnvelopes(modulationType type, float attack, float range, float curve);
	void setAttack(float attack);
	void setOffset(float offset);

	string getAudioName(FMOD_SOUND* sound);
	Layer* getLayerByName(string name);

	bool trigger = 0;
	float _gain = 1;
	float gainSnapshot = 1;

	// vectors holding collections of layers
	vector<Layer*> layerImpacts;
	vector<Layer*> layerLoops;
	vector<Layer*> pitchModLayers;

	ModulationData modData;

private:
	Envelopes attackEnv;


	float frequencyStandard = 44100;
};
