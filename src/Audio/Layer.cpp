#include "Layer.h"

//--------------------------------------------------------------
// baseLayer
//--------------------------------------------------------------

BaseLayer::BaseLayer(string label, FMOD_SYSTEM* system)
{
	this->label = label;
	this->system = system;
}

void BaseLayer::stopSounds()
{
	for (int i = 0; i < sounds.size(); i++) FMOD_Channel_Stop(channels[i]);
}

void BaseLayer::setVolume(float volume)
{
	for (int i = 0; i < sounds.size(); i++) FMOD_Channel_SetVolume(channels[i], volume);
}

void BaseLayer::setFrequency(float frequency)
{
	for (int i = 0; i < sounds.size(); i++) FMOD_Channel_SetFrequency(channels[i], frequency);
}

float BaseLayer::getFrequency()
{
	float out;
	for (int i = 0; i < sounds.size(); i++) FMOD_Channel_GetFrequency(channels[i], &out);
	return out;
}

//--------------------------------------------------------------
// impactLayer
//--------------------------------------------------------------

ImpactLayer::ImpactLayer(string label, FMOD_SYSTEM* system) : BaseLayer(label, system)
{
}

void ImpactLayer::startSounds()
{
	// Randomise sub and impact selection.
	int randValue = rand() % 3; 	
	FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sounds[randValue], false, &channels[randValue]);
}

//--------------------------------------------------------------
// loopLayer
//--------------------------------------------------------------

LoopLayer::LoopLayer(string label, FMOD_SYSTEM* system) : BaseLayer(label, system)
{
	positionPitchMod.curveType = linear;
}

void LoopLayer::startSounds()
{
	// Start all sounds in the 'sounds' vector.
	for (int i = 0; i < sounds.size(); i++) FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sounds[i], false, &channels[i]);
}

float LoopLayer::gainModulation(float inputValue, float positionTrigger, float timeTrigger, float actionTrigger, float actionInputValue)
{
	float actOuput = actionGainMod.calculateModulation(actionInputValue, actionTrigger);
	float output = positionGainMod.calculateModulation(inputValue, positionTrigger) * timeGainMod.calculateModulation(timeTrigger) * actOuput;

	return output;
}

float LoopLayer::pitchModulation(float inputValue, float positionTrigger, float timeTrigger)
{
	float output = positionPitchMod.calculateModulation(inputValue, positionTrigger);
	return output;
}