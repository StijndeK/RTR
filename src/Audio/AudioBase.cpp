#include "AudioBase.h"


void AudioBase::debugMessage(string message)
{
	std::cout << message << std::endl;
}

const char* AudioBase::getAudioPath(string filename)
{
	return ofToDataPath(filename).c_str();
}

void AudioBase::getDriverInfo(int& ofxFmodNumDevices, FMOD_SYSTEM* system, std::vector<std::string>& deviceNames)
{
	for (int i = 0; i < ofxFmodNumDevices; i++)
	{
		char name[256];
		FMOD_System_GetDriverInfo(system, i, name, 256, 0);
		deviceNames.push_back(name);
	}
}

// This sets the value to get to in the audio (the player position scaled to a value between 0 and 1).
// In the mock, there is no need to convert to a value between 0 and 1, so the currentPositionSlider value is used.
float AudioBase::setDecimalValue(ModulationData& data)
{
	float decimalValue = data.currentDistanceToGetTo;
	return decimalValue;
}

// Map value to a value between 0 and 1 in UE4.
// In the tool, use gain (and decimalValue in engine), because slider can be moved instantly opposite to the player that has to take time to move.
float AudioBase::setCurrentDistanceValue(float decimValue, float gainValue)
{
	return gainValue;
}


