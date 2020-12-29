#include "AudioSystem.h"

static FMOD_CHANNELGROUP* channelgroup;
static FMOD_SYSTEM* sys;

static int ofxFmodNumDevices = 0;
static int ofxFmodPreferedDevice = 0;
static int ofxFmodDevice = 0;
static unsigned int buffersize = 1024;

static std::vector<std::string> ofxFmodDeviceNames;

static bool audioLoaded = false;
static bool systemInitialised = false;

static const char* pathName;

//--------------------------------------------------------------
AudioSystem::AudioSystem()
{
}

AudioSystem::~AudioSystem()
{
}

//--------------------------------------------------------------
void AudioSystem::initFMODSystem() {
	if (systemInitialised == false) {
		// init
		FMOD_System_Create(&sys);

		// select device
		static bool listCreated = false;

		if (listCreated == false) {
			FMOD_System_GetNumDrivers(sys, &ofxFmodNumDevices);

			getDriverInfo();

			listCreated = true;
		}

		if (ofxFmodPreferedDevice < ofxFmodNumDevices) {
			ofxFmodDevice = ofxFmodPreferedDevice;
		}
		else {
			ofxFmodDevice = 0;
		}

		// set buffersize, keep number of buffers
		unsigned int bsTmp;
		int nbTmp;
		FMOD_System_GetDSPBufferSize(sys, &bsTmp, &nbTmp);
		FMOD_System_SetDSPBufferSize(sys, buffersize, nbTmp);

		FMOD_System_SetDriver(sys, ofxFmodDevice);

		FMOD_System_Init(sys, 2, FMOD_INIT_NORMAL, 0);
		FMOD_System_GetMasterChannelGroup(sys, &channelgroup);
		FMOD_System_Update(sys);
		systemInitialised = true;
	}
	loadAudio();
}

//--------------------------------------------------------------
void AudioSystem::update() {
	FMOD_System_Update(sys);
}

//--------------------------------------------------------------
void AudioSystem::loadAudio() {
	if (audioLoaded == false) {

		string filename = "TestSound.wav";
		static FMOD_RESULT result = FMOD_System_CreateSound(sys, ofToDataPath(filename).c_str(), FMOD_DEFAULT, 0, &sound);

		if (result != FMOD_OK) {
			debugMessage("Error: sound not loaded correctly");
		}
		audioLoaded = true;
	}
}

//--------------------------------------------------------------
void AudioSystem::playAudio() {
	FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sound, false, &channel);
	// FMOD_Channel_SetPaused(channel, false);
}

//--------------------------------------------------------------
void AudioSystem::setGain(float gain)
{
	FMOD_ChannelGroup_SetVolume(channelgroup, gain);
}

void AudioSystem::setPan(float p) {
	p = ofClamp(p, -1, 1);
	FMOD_Channel_SetPan(channel, p);
	FMOD_System_Update(sys);
}
// -------------------------
// Engine specific functions
// -------------------------

void AudioSystem::debugMessage(string message)
{
	std::cout << message << std::endl;
}

const char* AudioSystem::getAudioPath(string filename)
{
	pathName = ofToDataPath(filename).c_str();
	return pathName;
}

void AudioSystem::getDriverInfo()
{
	for (int i = 0; i < ofxFmodNumDevices; i++)
	{
		char name[256];
		FMOD_System_GetDriverInfo(sys, i, name, 256, 0);
		ofxFmodDeviceNames.push_back(name);
	}
}
