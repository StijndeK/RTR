#include "AudioSystem.h"

// General FMOD variables
// TODO: move these variables
static FMOD_CHANNELGROUP* channelgroup;
static FMOD_SYSTEM* sys;

static int ofxFmodNumDevices = 0;
static int ofxFmodPreferedDevice = 0;
static int ofxFmodDevice = 0;
static unsigned int buffersize = 1024;

static std::vector<std::string> ofxFmodDeviceNames;

static bool audioLoaded = false;
static bool systemInitialised = false;

AudioSystem::AudioSystem()
{
}

AudioSystem::~AudioSystem()
{
	sounds.clear();

	for (auto p : layerImpacts) {
		FMOD_Sound_Release(p->_sound);
		delete p;
	}
	layerImpacts.clear();

	// FMOD_Sound_Release(sounds[0]); // TODO: release all sounds
}

void AudioSystem::initFMODSystem() {
	if (systemInitialised == false) {
		// init
		FMOD_System_Create(&sys);

		// select device
		static bool listCreated = false;

		if (listCreated == false) {
			FMOD_System_GetNumDrivers(sys, &ofxFmodNumDevices);

			getDriverInfo(ofxFmodNumDevices, sys, ofxFmodDeviceNames);

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

void AudioSystem::update() {
	FMOD_System_Update(sys);
}

void AudioSystem::loadAudio() {
	if (audioLoaded == false) {
		ofDirectory dir("");
		dir.allowExt("wav");
		dir.listDir(); 	//populate the directory object

		//go through and print out all the paths
		for (int i = 0; i < dir.size(); i++) {

			// get path and name
			string tempName = dir.getName(i);
			tempName = tempName.at(0);
			debugMessage(tempName);

			// create sound
			 FMOD_SOUND* tempSound;
			 // sounds.push_back(tempSound);

			FMOD_System_CreateSound(sys, ofToDataPath(dir.getPath(i)).c_str(), FMOD_DEFAULT, 0, &tempSound);

			// initialise layers with their names and FMOD_SOUNDS
			if (tempName == "I") {
				layerImpacts.push_back(new Layer(tempSound, "Impact"));
			}
			else if (tempName == "S") {
				layerSubs.push_back(new Layer(tempSound, "Sub"));
			}
			else {
				layerTests.push_back(new Layer(tempSound, "Test"));
			}
		}

		audioLoaded = true;
	}
}

void AudioSystem::playAudio() {
	stopAudio();

	// randomise sub and impact selection
	int impactNumb = rand() % 3;
	int subNumb = rand() % 3;

	// print names
	debugMessage(getAudioName(layerImpacts[impactNumb]->_sound));
	debugMessage(getAudioName(layerSubs[subNumb]->_sound));
	debugMessage(getAudioName(layerTests[0]->_sound));

	// play sound
	FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, layerImpacts[impactNumb]->_sound, false, &channel);
	FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, layerSubs[subNumb]->_sound, false, &channel);
	FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, layerTests[0]->_sound, false, &channel);
}

void AudioSystem::stopAudio() {
	FMOD_ChannelGroup_Stop(channelgroup);
}

string AudioSystem::getAudioName(FMOD_SOUND* sound) {
	char name[256];
	FMOD_Sound_GetName(sound, name, 256);
	return name;
}

void AudioSystem::setGain(float gain)
{
	FMOD_ChannelGroup_SetVolume(channelgroup, gain);
}

void AudioSystem::setPan(float p) {
	p = ofClamp(p, -1, 1);
	FMOD_Channel_SetPan(channel, p);
	FMOD_System_Update(sys);
}