#include "Audio.h"

static FMOD_CHANNELGROUP* channelgroup;
static FMOD_SYSTEM* sys;

static int ofxFmodNumDevices = 0;
static int ofxFmodPreferedDevice = 0;
static int ofxFmodDevice = 0;
static unsigned int buffersize = 1024;

static std::vector<std::string> ofxFmodDeviceNames;

//--------------------------------------------------------------
void Audio::setup() {
	initFMOD();
	load();
}

//--------------------------------------------------------------
void Audio::update() {
	FMOD_System_Update(sys);
}

//--------------------------------------------------------------
void Audio::load() {
	string fileName = ofToDataPath("TestSound.wav");
	FMOD_RESULT result;

	// create sound
	result = FMOD_System_CreateSound(sys, fileName.c_str(), FMOD_SOFTWARE, 0, &sound);

	if (result != FMOD_OK) {
		std::cout << "sound not loaded correctly" << std::endl;
	}
	else {
		std::cout << "sound loaded correctly" << std::endl;
	}
}

//--------------------------------------------------------------
void Audio::play() {
	FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sound, false, &channel);
	FMOD_System_Update(sys);
	FMOD_Channel_SetPaused(channel, false);
}

void Audio::initFMOD() {

	FMOD_System_Create(&sys);

	// select device
	static bool listCreated = false;

	if (listCreated == false) {
		FMOD_System_GetNumDrivers(sys, &ofxFmodNumDevices);

		for (int i = 0; i < ofxFmodNumDevices; i++)
		{
			char name[256];
			FMOD_System_GetDriverInfo(sys, i, name, 256, 0);
			ofxFmodDeviceNames.push_back(name);
		}

		listCreated = true;

		cout << "--------------------------------\n";
		cout << "FMOD: Available Sound Outputs:\n";
		for (int i = 0; i < ofxFmodNumDevices; i++) { cout << i << " " << ofxFmodDeviceNames[i] << endl; }
		cout << "--------------------------------\n";
	}

	if (ofxFmodPreferedDevice < ofxFmodNumDevices) {
		ofxFmodDevice = ofxFmodPreferedDevice;
		cout << "Selected device = " << ofxFmodDevice << endl << endl;
	}
	else {
		cout << "Warning: selected device not available, use default" << endl << endl;
		ofxFmodDevice = 0;
	}

	// set buffersize, keep number of buffers
	unsigned int bsTmp;
	int nbTmp;
	FMOD_System_GetDSPBufferSize(sys, &bsTmp, &nbTmp);
	FMOD_System_SetDSPBufferSize(sys, buffersize, nbTmp);

	FMOD_System_SetDriver(sys, ofxFmodDevice);
	FMOD_System_SetSpeakerMode(sys, FMOD_SPEAKERMODE_7POINT1);

	FMOD_System_Init(sys, 2, FMOD_INIT_NORMAL, 0);
	FMOD_System_GetMasterChannelGroup(sys, &channelgroup);
	FMOD_System_Update(sys);
}