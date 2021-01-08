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
	modData.MockData();
	mainGainModulation.modType = exponential;
}

AudioSystem::~AudioSystem()
{
	for (auto layer : layerLoops) {
		for (auto s : layer->_sounds) {
			FMOD_Sound_Release(s);
		}
		delete layer;
	}
	layerLoops.clear();
	for (auto layer : layerImpacts) {
		for (auto s : layer->_sounds) {
			FMOD_Sound_Release(s);
		}
		delete layer;
	}
	layerImpacts.clear();
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

		debugMessage("Fmod initialised");
	}

	loadAudio();
}

void AudioSystem::update() {
	FMOD_System_Update(sys);

	if (playing) {
		// this value to get to in the audio == the player position scaled to a value between 0 and 1
		// float decimalValue = modData.ConvertToDecimalData(); 	// get value to get to from distance in float to value between 0 and 1 from distance values
		// in mock no need to convert, use the currentPositionSlider value (which is already between 0 and 1)
		float decimalValue = modData.currentDistanceToGetTo;

		// attack envelope
		float attackedGain = attackEnv.arAttackExp(_gain, envelopeTrigger);

		// modulate gain audio
		float outputGain = mainGainModulation.CalculateModulation(decimalValue, modulationTrigger);

		for (auto layer : layerLoops) {
			if (layer->_onOff) {
				float layerOutputGain = layer->gainMod.CalculateModulation(decimalValue, modulationTrigger);
				FMOD_Channel_SetVolume(layer->_channel, layerOutputGain * attackedGain * outputGain);
				//debugMessage(layer->_label + to_string(layerOutputGain * attackedGain * outputGain));
			}
		}

		// modulate pitch
		float frequencyRange = 1.5; // TODO: create UI setting for this value
		float outputPitch = mainPitchModulation.CalculateModulation(decimalValue, modulationTrigger);
		for (auto layer : pitchModLayers) {
			if (layer->_onOff) {
				float layerOutputPitch = layer->pitchMod.CalculateModulation(outputPitch, 1);
				FMOD_Channel_SetFrequency(layer->_channel, layerOutputPitch * (frequencyRange * frequencyStandard));
			}
		}

		// check if playing should end
		if (recordTimer) {
			float timerValue = stopTimer.timerTick();
			if (timerValue == 1) {
				stopRiser();
				recordTimer = false;
			}
		}
	}

	debugMessage(to_string(playing));
	
	// reset trigger for envelopes
	if (envelopeTrigger == 1) {
		debugMessage("trigger envelope");
		envelopeTrigger = 0;
	}
}

void AudioSystem::loadAudio() {
	if (audioLoaded == false) {
		// initialise layers
		int amountofLoopLayers = 5;
		string loopLayerNames[] = { "Pad: Start", "Pad: End", "Fx", "Noise", "Shepards" }; // TODO: use this voor UI as well
		for (int i = 0; i < amountofLoopLayers; i++) {
			layerLoops.push_back(new Layer(loopLayerNames[i], exponential ));
		}
		int amountOfImpactLayers = 2;
		string impactLayerNames[] = { "Hit", "Sub" }; // TODO: use this voor UI as well
		for (int i = 0; i < amountOfImpactLayers; i++) {
			layerImpacts.push_back(new Layer(impactLayerNames[i]));
		}

		// get directory
		ofDirectory dir("");
		dir.allowExt("wav");
		dir.listDir(); 	//populate the directory object

		// load sounds into layers
		for (int i = 0; i < dir.size(); i++) {

			// get path and name
			string tempName = dir.getName(i);

			// create sound
			FMOD_SOUND* tempSound;

			FMOD_System_CreateSound(sys, ofToDataPath(dir.getPath(i)).c_str(), FMOD_DEFAULT, 0, &tempSound);

			// initialise layers with their names and FMOD_SOUNDS
			if (tempName[0] == 'I') {			// Impact
				layerImpacts[0]->_sounds.push_back(tempSound);
				debugMessage("layerImpacts[0]: " + tempName);
			}
			else if (tempName[0] == 'S') {		// Sub
				layerImpacts[1]->_sounds.push_back(tempSound);
				debugMessage("layerImpacts[1]: " + tempName);
			}
			else if (tempName[0] == 'L') {		// Loop
				FMOD_Sound_SetMode(tempSound, FMOD_LOOP_NORMAL); // TODO: set mode at initialisation

				if (tempName[2] == 'P') {		// Loop: Start Pad
					layerLoops[0]->_sounds.push_back(tempSound);
					debugMessage("layerLoops[0]: " + tempName);
				}
				else if (tempName[2] == 'E') {	// Loop: End Pad
					layerLoops[1]->_sounds.push_back(tempSound);
					debugMessage("layerLoops[1]: " + tempName);
				}
				else if (tempName[2] == 'F') {	// Loop: Fx
					layerLoops[2]->_sounds.push_back(tempSound);
					debugMessage("layerLoops[2]: " + tempName);
				}
				else if (tempName[2] == 'N') {	// Loop: Noise
					layerLoops[3]->_sounds.push_back(tempSound);
					debugMessage("layerLoops[3]: " + tempName);
				}
				else if (tempName[2] == 'S') {	// Loop: Shepard
					layerLoops[4]->_sounds.push_back(tempSound);
					debugMessage("layerLoops[4]: " + tempName);
				}
				else {
					debugMessage("Error: Loopname not found: " + tempName);
				}
			}
			else {
				debugMessage("Error: Name not found: " + tempName);
			}
		}

		// set what values need to be modulated by what
		pitchModLayers.push_back(layerLoops[0]);
		pitchModLayers.push_back(layerLoops[1]);

		audioLoaded = true;
		debugMessage("audio loaded");

		FMOD_Channel_SetVolume(getLayerByName("Noise")->_channel, 0.2);
	}
}

void AudioSystem::startRiser()
{
	debugMessage("start riser");

	stopRiser();

	// create snapshot of gain for envelopes
	gainSnapshot = _gain;

	// set trigger bools
	modulationTrigger = 1;
	envelopeTrigger = 1;
	playing = true;
	recordTimer = false;

	playAudioLoops();
}

void AudioSystem::stopRiser()
{
	debugMessage("stop riser");

	stopAudioLayers(layerLoops);
	stopAudioLayers(layerImpacts);

	playing = false;
}

void AudioSystem::playAudioLoops() {
	debugMessage("Now Playing Loops: ");
	for (auto layer : layerLoops) {
		// check if layer is on
		if (layer->_onOff) {
			FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, layer->_sounds[layer->_currentSoundIdentifier], false, &layer->_channel);
			debugMessage("	" + layer->_label + ":");
			debugMessage("		" + getAudioName(layer->_sounds[layer->_currentSoundIdentifier]));
		}
	}
}

void AudioSystem::playAudioImpacts() {
	debugMessage("Now Playing Impacts: ");
	for (auto layer : layerImpacts) {
		int randValue = rand() % 3; 	// randomise sub and impact selection
		FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, layer->_sounds[randValue], false, &layer->_channel);
		debugMessage(getAudioName(layer->_sounds[randValue]));
	}
}

void AudioSystem::startRelease() {
	debugMessage("start stopping audio");
	modulationTrigger = 0;
	recordTimer = true;
	playAudioImpacts();

	// check if trigger needs to be set to 0 immediatly, or if there is an offset: if the riser can still rise a bit. and iddeally update the speed of the riser to reach the full end for the impact

	// after release is done completely stop the sounds
}

void AudioSystem::stopAudioLayers(vector<Layer*> layersToStop) {
	debugMessage("stop Audio");
	for (auto layer : layersToStop) {
		FMOD_Channel_Stop(layer->_channel);
	}
}

void AudioSystem::setGain(float gain) {
	debugMessage("setGain: " + to_string(gain));
	_gain = pow(10, gain / 20);
}

void AudioSystem::setModulation(modulationParameter type, float attack, float range, float curve)
{
	float release = 2000; // TODO: get release from UI

	if (type == Amp) {
		debugMessage("setModulation: Amp. " + to_string(attack));
		mainGainModulation.CalculateStepSize(attack, attack * 1.5, release);
		for (auto layer : layerLoops) {
			layer->gainMod.CalculateStepSize(attack, attack * 1.5, release);
		}

		// set timer length
		stopTimer.setLength(release);
	}
	else if (type == Pitch) {
		debugMessage("setModulation: Pitch. " + to_string(attack));
		mainPitchModulation.CalculateStepSize(attack, attack * 1.5, release);
		for (auto layer : pitchModLayers) {
			layer->pitchModulationRange = range;
			layer->pitchMod.CalculateStepSize(attack, attack * 1.5, release);
			// TODO amp start value based on curve
		}
	}
}

void AudioSystem::setAttack(float attack) {
	debugMessage("setAttack: " + to_string(attack));
	attackEnv.setARExp(attack, 0);
}

void AudioSystem::setOffset(float offset) {
	// check if offset is necessary
	// check if max offset is necessary (maybe only needs half of the time, as the sound does need to get more intense)
	// set values for offset to happen and sound to modulate to its conclusion
}

string AudioSystem::getAudioName(FMOD_SOUND* sound) {
	char name[256];
	FMOD_Sound_GetName(sound, name, 256);
	string outName = name;
	return name;
}

Layer* AudioSystem::getLayerByName(string name) {
	for (auto l : layerLoops) {
		if (l->_label == name) {
			debugMessage("getLayerByName: " + l->_label);
			return l;
		}
	}
}