#pragma once
#include "ofMain.h"
#include "fmod.h"
#include <string>
#include "Modulation/Envelopes.h"
#include "Modulation/Modulation.h"
#include "Modulation/ModulationTypes.h"

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

	float gainModulation(float inputValue, float positionTrigger, float timeTrigger);

	float pitchModulation(float inputValue, float positionTrigger, float timeTrigger);

	Modulation positionGainMod;
	Modulation positionPitchMod;
	Modulation timeGainMod;

	// set if pitch mod should be on, main gain mod and less gain mod is always on
	bool mainPitchModToggle = false;
	float frequencyRange = 1.5;
};

