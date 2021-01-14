#pragma once
#include "fmod.h"
#include "ofMain.h"
#include "Modulation/ModulationData.h"

class AudioBase
{
public:
	static void debugMessage(string message);
	static const char* getAudioPath(string filename);
	static void getDriverInfo(int& ofxFmodNumDevices, FMOD_SYSTEM* system, std::vector<std::string>& deviceNames);
	static float setDecimalValue(ModulationData& data);
	static float setCurrentDistanceValue(float decimValue, float gainValue);
private:

};

