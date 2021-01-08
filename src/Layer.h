#pragma once
#include "ofMain.h"
#include "fmod.h"
#include <string>
#include "Envelopes.h"
#include "Modulation.h"
#include "ModulationTypes.h"

class Layer : public ModulationTypes
{
public:
	Layer(string label, modulationType modType);
	~Layer();

	vector<FMOD_SOUND*> _sounds;
	FMOD_CHANNEL* _channel;
	string _label;
	int _currentSoundIdentifier = 0;
	bool _onOff = 0;
	Envelopes gainEnv;
	Envelopes pitchEnv;
	float ampModulationRange;
	float pitchModulationRange;
	Modulation gainMod;
	Modulation pitchMod;
};

