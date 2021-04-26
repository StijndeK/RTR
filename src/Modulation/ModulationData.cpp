#include "ModulationData.h"

//TODO: capitalisation

// Set values that would be received from the game manually.
void ModulationData::mockData() 
{
	positionGoal = 10000;
	positionStart = 2000;
	totalDistance = positionGoal - positionStart;
}

// Get the value to get to as a float between 0 and 1 (to feed into FMOD_setVolume).
float ModulationData::convertToDecimalData(float position) 
{
	return 1 - ((currentDistanceToGetTo - positionStart) / totalDistance);
}