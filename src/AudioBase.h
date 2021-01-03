#pragma once
#include "fmod.h"
#include "ofMain.h"

class AudioBase
{
public:
	AudioBase();
	~AudioBase();

	void debugMessage(string message);
	const char* getAudioPath(string filename);
	void getDriverInfo(int ofxFmodNumDevices, FMOD_SYSTEM* system, std::vector<std::string> deviceNames);

private:

};

