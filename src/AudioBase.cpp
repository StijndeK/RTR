#include "AudioBase.h"

AudioBase::AudioBase()
{
}

AudioBase::~AudioBase()
{
}

void AudioBase::debugMessage(string message)
{
	std::cout << message << std::endl;
}

const char* AudioBase::getAudioPath(string filename)
{
	return ofToDataPath(filename).c_str();;
}

void AudioBase::getDriverInfo(int ofxFmodNumDevices, FMOD_SYSTEM* system, std::vector<std::string> deviceNames)
{
	for (int i = 0; i < ofxFmodNumDevices; i++)
	{
		char name[256];
		FMOD_System_GetDriverInfo(system, i, name, 256, 0);
		deviceNames.push_back(name);
	}
}
