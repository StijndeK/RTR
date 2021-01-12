#pragma once
#include "ofMain.h"
#include "fmod.h"
#include "Layer.h"
#include "AudioBase.h"
#include "Modulation/Envelopes.h"
#include "Modulation/Modulation.h"
#include "Modulation/ModulationData.h"
#include "Modulation/Timer.h"

class AudioSystem : public AudioBase, public ModulationTypes {

public:
	AudioSystem();
	~AudioSystem();

	//enum modulationParameter { Amp, Pitch};

	void initFMODSystem();
	void loadAudio();

	void update();

	void startRiser();
	void startRelease();
	void stopRiser();
	void startAudioLayers(vector<ImpactLayer*> layersToStart);
	void startAudioLayers(vector<LoopLayer*> layersToStart);
	void stopAudioLayers(vector<LoopLayer*> layersToStop);
	void stopAudioLayers(vector<ImpactLayer*> layersToStop);

	// setters
	// TODO: maybe move all setters to layers
	void setGain(float gain);
	void setGainModulation(float attack);
	void setPitchModulation(float attack);
	void setAttack(float attack);
	void setOffset(float offset);
	void setTimer(float slowdownTimeMs, float slowDownAmount = 1);
	void checkLessModifier(float value);

	// getters
	string getAudioName(FMOD_SOUND* sound);
	LoopLayer* getLayerByName(string name);

	bool modulationTrigger = 0;			// true on attack when playing
	bool envelopeTrigger = 0;	// true on start, then immediatly false
	bool playing = false;		// true while audio is playing
	bool recordTimer = false;	// true while release is playing, to get notified when to stop audio

	float _gain = 1;
	float gainSnapshot = 1;

	// holds values to check deviation for lessGain
	vector<float> lastValues; // TODO: fill immediatly
	int currentValue = 0;
	int currentTick = 0;

	// vectors holding collections of layers initialised in load audio
	vector<ImpactLayer*> layerImpacts;
	vector<LoopLayer*> layerLoops;

	// data on the modulation, such as player position etc
	ModulationData modData;

	// values for UI plotters
	float mainOutputGainAllLayers;
	float mainFrequencyAllLayers;
private:
	// attack only envelope for the start of the sound
	Envelopes attackEnv;

	// Timer on when to stop all audio after impact
	Timer stopTimer;
	// Timer to check time after start
	Timer timePlaying;

	float frequencyStandard = 44100;
};
