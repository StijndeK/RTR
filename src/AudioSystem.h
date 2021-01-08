#pragma once
#include "ofMain.h"
#include "fmod.h"
#include "Layer.h"
#include "AudioBase.h"
#include "Envelopes.h"
#include "Modulation.h"
#include "ModulationData.h"
#include "Timer.h"

class AudioSystem : public AudioBase, public ModulationTypes {

public:
	AudioSystem();
	~AudioSystem();

	enum modulationParameter { Amp, Pitch};

	void initFMODSystem();
	void update();
	void loadAudio();

	void startRiser();
	void stopRiser();
	void playAudioLoops();
	void playAudioImpacts();
	void startRelease();
	void stopAudioLayers(vector<Layer*> layersToStop);

	void setGain(float gain);
	void setModulation(modulationParameter type, float attack, float range, float curve);
	void setAttack(float attack);
	void setOffset(float offset);

	string getAudioName(FMOD_SOUND* sound);
	Layer* getLayerByName(string name);

	bool modulationTrigger = 0;			// true on attack when playing
	bool envelopeTrigger = 0;	// true on start, then immediatly false
	bool playing = false;		// true while audio is playing
	bool recordTimer = false;	// true while release is playing, to get notified when to stop audio
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
	Timer stopTimer;

	float frequencyStandard = 44100;
};
