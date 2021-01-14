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

	static void startRiser();
	void startRelease();
	static void stopRiser();
	static void startAudioLayers(vector<ImpactLayer*> layersToStart);
	static void startAudioLayers(vector<LoopLayer*> layersToStart);
	static void stopAudioLayers(vector<LoopLayer*> layersToStop);
	static void stopAudioLayers(vector<ImpactLayer*> layersToStop);

	// setters
	// TODO: maybe move all setters to layers
	void setGain(float gain);
	void setGainModulation(float attack);
	void setPitchModulation(float attack);
	void setAttack(float attack);
	void setRelease(float release);
	void setOffset(float offset);
	void setModulationCurve(float startValue);
	void setTimer(float slowdownTimeMs, float slowDownAmount = 1);

	void checkLessModifier(float value);

	// getters
	string getAudioName(FMOD_SOUND* sound);
	LoopLayer* getLayerByName(string name);

	static bool audioLoaded;
	static bool systemInitialised;

	static bool modulationTrigger;			// true on attack when playing
	static bool envelopeTrigger;	// true on start, then immediatly false
	static bool playing;		// true while audio is playing
	static bool recordTimer;	// true while release is playing, to get notified when to stop audio

	static float _gain;
	static float gainSnapshot;

	// holds values to check deviation for lessGain
	vector<float> lastValues; // TODO: fill immediatly
	int currentValue = 0;
	int currentTick = 0;

	// vectors holding collections of layers initialised in load audio
	static vector<ImpactLayer*> layerImpacts;
	static vector<LoopLayer*> layerLoops;

	// data on the modulation, such as player position etc
	ModulationData modData;

	// values for UI plotters
	float mainOutputGainAllLayers;
	float mainFrequencyAllLayers;

	// FMOD
	static int ofxFmodNumDevices;
	static int ofxFmodPreferedDevice;
	static int ofxFmodDevice;
	static unsigned int buffersize;

	static std::vector<std::string> ofxFmodDeviceNames;

	static FMOD_CHANNELGROUP* channelgroup;
	static FMOD_SYSTEM* sys;
private:
	// attack only envelope for the start of the sound
	Envelopes attackEnv;

	// Timer on when to stop all audio after impact
	Timer stopTimer;
	// Timer to check time after start
	Timer timePlaying;

	float frequencyStandard = 44100;
};
