#pragma once
#include "ofMain.h"
#include "fmod.h"
#include "Layer.h"
#include "AudioBase.h"
#include "Envelopes.h"
#include "Modulation.h"
#include "ModulationData.h"

class AudioSystem : public AudioBase, public ModulationTypes {

public:
	AudioSystem();
	~AudioSystem();

	enum modulationParameter { Amp, Pitch};

	void initFMODSystem();
	void update();
	void loadAudio();
	void playAudioLoops();
	void playAudioImpacts();
	void playEnvelopes();
	void startStopping();
	void stopAudio(vector<Layer*> layersToStop);

	void setGain(float gain);
	void setPan(float p);
	void setEnvelopes(modulationParameter type, float attack, float range, float curve);
	void setModulation(modulationParameter type, float attack, float range, float curve);
	void setAttack(float attack);
	void setOffset(float offset);

	string getAudioName(FMOD_SOUND* sound);
	Layer* getLayerByName(string name);

	bool trigger = 0;
	bool envelopeTrigger = 0;
	float _gain = 1;
	float gainSnapshot = 1;

	// vectors holding collections of layers initialised in load audio
	vector<Layer*> layerImpacts;
	vector<Layer*> layerLoops;

	// vectors grouping types of modulation
	vector<Layer*> pitchModLayers;
	Modulation mainGainModulation;
	Modulation mainPitchModulation;

	ModulationData modData;
private:
	Envelopes attackEnv;


	float frequencyStandard = 44100;
};
