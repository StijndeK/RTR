#pragma once
#include "ofMain.h"
#include "fmod.h"
#include <string>
#include "Modulation/Envelopes.h"
#include "Modulation/Modulation.h"
#include "Modulation/ModulationTypes.h"

// TODO: add curve editing (by calculating max and min as with transitionfx (now exp or linear)) and range for the modulation (min and max value for the audio (now 0 and 1))

class BaseLayer : public ModulationTypes
{
public:
	BaseLayer(string label, FMOD_SYSTEM* system);
	~BaseLayer();

	FMOD_SYSTEM* _system;

	void stopSounds();
	void setVolume(float volume);
	void setFrequency(float frequency);
	float getFrequency();

	vector<FMOD_SOUND*> _sounds;
	// TODO: automatically assign channels to vector
	vector<FMOD_CHANNEL*> _channels;
	FMOD_CHANNEL* _channel;
	FMOD_CHANNEL* channel2;
	FMOD_CHANNEL* channel3;
	FMOD_CHANNEL* channel4;
	FMOD_CHANNEL* channel5;

	string _label;
	int _currentSoundIdentifier = 0;
	bool _onOff = 0;
};

class ImpactLayer : public BaseLayer
{
public:
	ImpactLayer(string label, FMOD_SYSTEM* system);
	~ImpactLayer();

	void startSounds();
};

class LoopLayer : public BaseLayer
{
public:
	LoopLayer(string label, FMOD_SYSTEM* system);
	~LoopLayer();

	void startSounds();

	Modulation mainGainMod;
	Modulation mainPitchMod;
	Modulation lessGainMod;

	// set if pitch mod should be on, main gain mod and less gain mod is always on
	bool mainPitchModToggle = false;
	float frequencyRange = 1.5; // TODO: create UI setting for this value
};
