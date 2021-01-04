#pragma once
#include "fmod.h"
#include "ofMain.h"

class AudioBase
{
public:
	AudioBase();
	~AudioBase();

	static void debugMessage(string message);
	static const char* getAudioPath(string filename);
	static void getDriverInfo(int& ofxFmodNumDevices, FMOD_SYSTEM* system, std::vector<std::string>& deviceNames);

private:

};

