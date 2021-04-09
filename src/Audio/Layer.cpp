#include "Layer.h"

//--------------------------------------------------------------
// baseLayer
//--------------------------------------------------------------

BaseLayer::BaseLayer(string label, FMOD_SYSTEM* system)
{
	this->label = label;
	this->system = system;
}

BaseLayer::~BaseLayer()
{
}

void BaseLayer::stopSounds()
{
	for (int i = 0; i < sounds.size(); i++) {
		FMOD_Channel_Stop(channels[i]);
	}
}

void BaseLayer::setVolume(float volume)
{
	for (int i = 0; i < sounds.size(); i++) {
		FMOD_Channel_SetVolume(channels[i], volume);
	}
}

void BaseLayer::setFrequency(float frequency)
{
	for (int i = 0; i < sounds.size(); i++) {
		FMOD_Channel_SetFrequency(channels[i], frequency);
	}
}

float BaseLayer::getFrequency()
{
	float out = 0;
	for (int i = 0; i < sounds.size(); i++) {
		FMOD_Channel_GetFrequency(channels[i], &out);
	}
	return out;
}

//--------------------------------------------------------------
// impactLayer
//--------------------------------------------------------------

ImpactLayer::ImpactLayer(string label, FMOD_SYSTEM* system) : BaseLayer(label, system)
{
}

ImpactLayer::~ImpactLayer()
{
}

void ImpactLayer::startSounds()
{
	int randValue = rand() % 3; 	// randomise sub and impact selection
	FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sounds[randValue], false, &channels[randValue]);
}

//--------------------------------------------------------------
// loopLayer
//--------------------------------------------------------------

LoopLayer::LoopLayer(string label, FMOD_SYSTEM* system) : BaseLayer(label, system)
{
	positionPitchMod.modType = linear;
}

LoopLayer::~LoopLayer()
{
}

void LoopLayer::startSounds()
{
	for (int i = 0; i < sounds.size(); i++) {
		FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sounds[i], false, &channels[i]);
	}
}

float LoopLayer::gainModulation(float inputValue, float positionTrigger, float timeTrigger, float actionTrigger, float actionInputValue)
{
	float actOuput = actionGainMod.CalculateModulation(actionInputValue, actionTrigger);
	float output = positionGainMod.CalculateModulation(inputValue, positionTrigger) * timeGainMod.CalculateModulation(timeTrigger) * actOuput;

	return output;
}

float LoopLayer::pitchModulation(float inputValue, float positionTrigger, float timeTrigger)
{
	float output = positionPitchMod.CalculateModulation(inputValue, positionTrigger);
	return output;


	enum MyEnum
	{
		test,
		twest
	};
}