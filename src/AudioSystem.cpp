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
	for (auto p : layerImpacts) {
		FMOD_Sound_Release(p->_sound);
		delete p;
	}
	for (auto p : layerSubs) {
		FMOD_Sound_Release(p->_sound);
		delete p;
	}
	for (auto p : layerStartPads) {
		FMOD_Sound_Release(p->_sound);
		delete p;
	}
	for (auto p : layerEndPads) {
		FMOD_Sound_Release(p->_sound);
		delete p;
	}
	for (auto p : layerFx) {
		FMOD_Sound_Release(p->_sound);
		delete p;
	}
	for (auto p : layerNoises) {
		FMOD_Sound_Release(p->_sound);
		delete p;
	}
	for (auto p : layerShepards) {
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

		FMOD_System_Init(sys, 104, FMOD_INIT_NORMAL, 0);
		FMOD_System_GetMasterChannelGroup(sys, &channelgroup);
		FMOD_System_Update(sys);
		systemInitialised = true;
	}
	loadAudio();
}

void AudioSystem::update() {
	FMOD_System_Update(sys);

	// amplitude modulation
	float attackedGain = attackEnv.arAttackExp(_gain, trigger);	// initial attack envelope
	debugMessage(to_string(attackedGain));
	float envelopeGain = rangeEnv.arLin(attackedGain, trigger); 	// amplitude modulation
	debugMessage(to_string(envelopeGain));
	FMOD_ChannelGroup_SetVolume(channelgroup, envelopeGain);
	
	// pitch modulation

	// reset trigger
	if (trigger == 1) {
		trigger = 0;
	}

}

void AudioSystem::loadAudio() {
	if (audioLoaded == false) {
		ofDirectory dir("");
		dir.allowExt("wav");
		dir.listDir(); 	//populate the directory object

		//go through and print out all the paths
		for (int i = 0; i < dir.size(); i++) {

			// get path and name
			string tempName = dir.getName(i).substr(0, 3);

			// create sound
			FMOD_SOUND* tempSound;

			FMOD_System_CreateSound(sys, ofToDataPath(dir.getPath(i)).c_str(), FMOD_DEFAULT, 0, &tempSound);

			// initialise layers with their names and FMOD_SOUNDS
			if (tempName[0] == 'I') {			// Impact
				layerImpacts.push_back(new Layer(tempSound, "Impact"));
			}
			else if (tempName[0] == 'S') {		// Sub
				layerSubs.push_back(new Layer(tempSound, "Sub"));
			}
			else if (tempName[0] == 'L') {		// Loop
				FMOD_Sound_SetMode(tempSound, FMOD_LOOP_NORMAL); // TODO: set mode at initialisation

				if (tempName[2] == 'P') {		// Loop: Start Pad
					layerStartPads.push_back(new Layer(tempSound, "Loop: Start Pad"));
				}
				else if (tempName[2] == 'E') {		// Loop: End Pad
					layerEndPads.push_back(new Layer(tempSound, "Loop: End Pad"));
				}
				else if (tempName[2] == 'F') {	// Loop: Fx
					layerFx.push_back(new Layer(tempSound, "Loop: Fx"));
				}
				else if (tempName[2] == 'N') {	// Loop: Noise
					layerNoises.push_back(new Layer(tempSound, "Loop: Noise"));
				}
				else if (tempName[2] == 'S') {	// Loop: Shepard
					layerShepards.push_back(new Layer(tempSound, "Loop: Shepard"));
				}
				else {

				}
			}
			else {
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
	debugMessage("Now Playing: ");
	debugMessage(getAudioName(layerImpacts[impactNumb]->_sound));
	debugMessage(getAudioName(layerSubs[subNumb]->_sound));
	debugMessage(getAudioName(layerStartPads[0]->_sound));
	debugMessage(getAudioName(layerEndPads[0]->_sound));
	debugMessage(getAudioName(layerFx[0]->_sound));
	debugMessage(getAudioName(layerNoises[0]->_sound));
	debugMessage(getAudioName(layerShepards[0]->_sound));

	// set envelopes
	// create snapshot of gain for envelopes
	gainSnapshot = _gain;
	// trigger envelopes
	trigger = 1;

	// play sound
	// FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, layerImpacts[impactNumb]->_sound, false, &channel);
	// FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, layerSubs[subNumb]->_sound, false, &channel);
	FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, layerStartPads[0]->_sound, false, &channel);
	FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, layerEndPads[0]->_sound, false, &channel);
	FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, layerFx[0]->_sound, false, &channel);
	FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, layerNoises[1]->_sound, false, &channel);
	FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, layerShepards[0]->_sound, false, &channel);
}

void AudioSystem::stopAudio() {
	trigger = 1;
	FMOD_ChannelGroup_Stop(channelgroup);
}

string AudioSystem::getAudioName(FMOD_SOUND* sound) {
	char name[256];
	FMOD_Sound_GetName(sound, name, 256);
	return name;
}

void AudioSystem::setGain(float gain)
{
	_gain = pow(10, gain / 20);
	// FMOD_ChannelGroup_SetVolume(channelgroup, gain);
}

void AudioSystem::setPan(float p) {
	p = ofClamp(p, -1, 1);
	FMOD_Channel_SetPan(channel, p);
	FMOD_System_Update(sys);
}

void AudioSystem::setEnvelope(float attack) {
	debugMessage("env: " + to_string(attack));
	rangeEnv.setARLinear(attack, 500); // use default release value, to be replaced with new release slider input
}

void AudioSystem::setAttack(float attack)
{
	debugMessage("att: " + to_string(attack));
	attackEnv.setARExp(attack, 500);
}
