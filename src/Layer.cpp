#include "Layer.h"

Layer::Layer(FMOD_SOUND* sound, std::string label)
{
	_label = label;
	_sound = sound;
}

Layer::~Layer()
{
}