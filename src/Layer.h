#pragma once
#include "ofMain.h"
#include "fmod.h"
#include <string>

class Layer
{
public:
	Layer(string label);
	~Layer();

	vector<FMOD_SOUND*> _sounds;
	FMOD_CHANNEL* _channel;
	string _label;
	int _currentSoundIdentifier = 0;
	bool _onOff = 0;
private:

};

