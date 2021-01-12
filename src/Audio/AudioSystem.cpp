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

	setTimer(5000);
	vector<float> vect(40, 0.f);
	lastValues = vect;
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

	FMOD_ChannelGroup_SetVolume(channelgroup, 0);
	loadAudio();
}

void AudioSystem::loadAudio() {
	if (audioLoaded == false) {
		// initialise layers
		int amountofLoopLayers = 5;
		string loopLayerNames[] = { "Pad: Start", "Pad: End", "Fx", "Noise", "Shepards" }; // TODO: use this voor UI as well
		for (int i = 0; i < amountofLoopLayers; i++) {
			layerLoops.push_back(new LoopLayer(loopLayerNames[i], sys));
		}

		int amountOfImpactLayers = 2;
		string impactLayerNames[] = { "Hit", "Sub" }; // TODO: use this voor UI as well
		for (int i = 0; i < amountOfImpactLayers; i++) {
			layerImpacts.push_back(new ImpactLayer(impactLayerNames[i], sys));
		}

		// get directory
		ofDirectory dir("");
		dir.allowExt("wav");
		dir.listDir(); 	//populate the directory object

		// load sounds into layers
		for (int i = 0; i < dir.size(); i++) {

			// get path and name
			string tempName = dir.getName(i);

			// create sound and channel to add to layer
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

		// set on for frequency
		getLayerByName("Pad: Start")->mainPitchModToggle = true;
		getLayerByName("Pad: End")->mainPitchModToggle = true;

		audioLoaded = true;
		debugMessage("audio loaded");

		FMOD_Channel_SetVolume(getLayerByName("Noise")->_channel, 0.2);
	}
}

void AudioSystem::update() {
	FMOD_System_Update(sys);

	if (playing) {
		// update timer
		timePlaying.timerTick();

		// this value to get to in the audio == the player position scaled to a value between 0 and 1
		// float decimalValue = modData.ConvertToDecimalData(); 	// get value to get to from distance in float to value between 0 and 1 from distance values
		// in mock no need to convert, use the currentPositionSlider value (which is already between 0 and 1)
		float decimalValue = modData.currentDistanceToGetTo;

		// attack envelope
		float attackedGain = attackEnv.arAttackExp(_gain, envelopeTrigger);

		// plotting
		int onLayers = 0;
		mainOutputGainAllLayers = 0;
		mainFrequencyAllLayers = 0;

		for (auto layer : layerLoops) {
			if (layer->_onOff) {
				// gain modulation
				float outputGain = layer->mainGainMod.CalculateModulation(decimalValue, modulationTrigger);
				layer->setVolume(attackedGain * outputGain);

				// plotting
				onLayers++;
				mainOutputGainAllLayers += attackedGain * outputGain;
				mainFrequencyAllLayers += layer->getFrequency();

				// pitch modulation
				if (layer->mainPitchModToggle) {
					float outputPitch = layer->mainPitchMod.CalculateModulation(decimalValue, modulationTrigger);
					layer->setFrequency(outputPitch * (layer->frequencyRange * frequencyStandard));
				}
			}
		}

		// plotting
		mainOutputGainAllLayers /= onLayers;
		mainFrequencyAllLayers /= onLayers;

		// check every 10 ticks to set lessGain
		if (currentTick == 10) {
			// TODO: map value to a value between 0 and 1 in UE4
			// TODO: check if in tool or in engine
			// if in tool, use actuall gain, because slider can be moved instantly
			float currentDistanceValue = mainOutputGainAllLayers;
			// else
			//float currentDistanceValue = modData.currentDistanceToGetTo;
			checkLessModifier(currentDistanceValue);
			currentTick = 0;
		}
		else {
			currentTick++;
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
	
	// reset trigger for envelopes
	if (envelopeTrigger == 1) {
		debugMessage("trigger envelope");
		envelopeTrigger = 0;
	}
}

void AudioSystem::startRiser()
{
	stopRiser();

	debugMessage("start riser");

	// create snapshot of gain for envelopes
	gainSnapshot = _gain;

	// set trigger bools
	modulationTrigger = 1;
	envelopeTrigger = 1;
	playing = true;
	recordTimer = false;

	FMOD_ChannelGroup_SetVolume(channelgroup, 1);
	startAudioLayers(layerLoops);
}

// begin release phase of riser, with timer that checks when to completely stop the riser
void AudioSystem::startRelease() {
	// check if offset is necessary
	// check if max offset is necessary (maybe only needs half of the time, as the sound does need to get more intense)
	// set values for offset to happen and sound to modulate to its conclusion

	debugMessage("start stopping audio");
	modulationTrigger = 0;
	recordTimer = true;

	// play impacts
	startAudioLayers(layerImpacts);
}

void AudioSystem::stopRiser()
{
	debugMessage("stop riser");

	stopAudioLayers(layerLoops);
	stopAudioLayers(layerImpacts);
	FMOD_ChannelGroup_SetVolume(channelgroup, 0);

	playing = false;
}

void AudioSystem::startAudioLayers(vector<LoopLayer*> layersToStart) {
	debugMessage("start Audio");
	for (auto layer : layersToStart) {
		if (layer->_onOff) {
			layer->startSounds();
		}
	}
}

void AudioSystem::startAudioLayers(vector<ImpactLayer*> layersToStart) {
	debugMessage("start Audio");
	for (auto layer : layersToStart) {
		layer->startSounds();
	}
}

void AudioSystem::stopAudioLayers(vector<LoopLayer*> layersToStop) {
	debugMessage("stop Audio");
	for (auto layer : layersToStop) {
		layer->stopSounds();
	}
}

void AudioSystem::stopAudioLayers(vector<ImpactLayer*> layersToStop) {
	debugMessage("stop Audio");
	for (auto layer : layersToStop) {
		layer->stopSounds();
	}
}


void AudioSystem::setGain(float gain) {
	debugMessage("setGain: " + to_string(gain));
	_gain = pow(10, gain / 20);
}

void AudioSystem::setGainModulation(float attack)
{
	float release = 2000; // TODO: get release from UI
	// TODO:  float range, float curve

	debugMessage("setModulation: Amp. " + to_string(attack));

	for (auto layer : layerLoops) {
		layer->mainGainMod.CalculateStepSize(attack, attack * 1.5, release);
	}

	// set timer length
	stopTimer.setLength(release + 50);
}

void AudioSystem::setPitchModulation(float attack)
{
	float release = 2000; // TODO: get release from UI
	// TODO:  float range, float curve

	debugMessage("setModulation: Pitch. " + to_string(attack));

	for (auto layer : layerLoops) {
		if (layer->mainPitchModToggle) {
			layer->mainPitchMod.CalculateStepSize(attack, attack * 1.5, release);
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

// after how long should the riser slowdown when goal hasnt been reached yet
// generally this point should not be reached as action over time is checked by the action timer, but due to players maybe going afk etc, you might want the timer to slow down after a long amount of time
void AudioSystem::setTimer(float slowdownTimeMs, float slowDownAmount) {
	timePlaying.setLength(slowdownTimeMs);
}

// after how long and how much defiation in player position should the riser slow down
void AudioSystem::checkLessModifier(float value) {
	float deviationValue = 0.5; // if only distance moved changed half, start decreasing intensity of the riser

	lastValues[currentValue] = value;

	// set value in currentValue
	currentValue = (currentValue + 1) % lastValues.size();

	float currentDeviation = 0;
	for (float v : lastValues) {
		currentDeviation += v;
	}
	currentDeviation /= lastValues.size();

	float threshold = 0.1; // TODO: set this value from input

	if (currentDeviation - value <= threshold && currentDeviation - value >= 0 - threshold) {
		debugMessage("current deviation" + to_string(currentDeviation) = " | value: " + to_string(value));
	}
	// check only for if its bigger
	// then check if has not changed enough
}

string AudioSystem::getAudioName(FMOD_SOUND* sound) {
	char name[256];
	FMOD_Sound_GetName(sound, name, 256);
	string outName = name;
	return name;
}

LoopLayer* AudioSystem::getLayerByName(string name) {
	for (auto l : layerLoops) {
		if (l->_label == name) {
			debugMessage("getLayerByName: " + l->_label);
			return l;
		}
	}
}