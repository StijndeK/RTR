#pragma once
#include "ofMain.h"
#include "fmod.h"
#include <string>

class Layer
{
public:
	Layer(FMOD_SOUND* sound, std::string label);
	~Layer();

	FMOD_SOUND* _sound;
	std::string _label;
private:

};

