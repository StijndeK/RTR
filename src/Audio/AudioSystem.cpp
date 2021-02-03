#include "AudioSystem.h"

// initialise static FMOD variables
FMOD_CHANNELGROUP* AudioSystem::channelgroup;
FMOD_SYSTEM* AudioSystem::sys;

int AudioSystem::ofxFmodNumDevices = 0;
int AudioSystem::ofxFmodPreferedDevice = 0;
int AudioSystem::ofxFmodDevice = 0;
unsigned int AudioSystem::buffersize = 1024;

std::vector<std::string> AudioSystem::ofxFmodDeviceNames;

// Initialise other static members
bool AudioSystem::audioLoaded = false;
bool AudioSystem::systemInitialised = false;

int AudioSystem::currentValue = 0;
int AudioSystem::currentTick = 0;
vector<float> AudioSystem::lastValues;

float AudioSystem::mainOutputGainAllLayers;
float AudioSystem::mainFrequencyAllLayers;

float AudioSystem::_gain = 1;
float AudioSystem::gainSnapshot = 1;

bool AudioSystem::modulationTrigger = 0;			// true on attack when playing
bool AudioSystem::timeModulationTrigger = 0;		// true after timeModulation threshold is passed
bool AudioSystem::envelopeTrigger = 0;				// true on start, then immediatly false
bool AudioSystem::playing = false;					// true while audio is playing
vector<ImpactLayer*> AudioSystem::layerImpacts;
vector<LoopLayer*> AudioSystem::layerLoops;

float AudioSystem::frequencyStandard = 44100;

Envelopes AudioSystem::attackEnv;
Timer AudioSystem::releaseTimer;
Timer AudioSystem::timeModulationTimer;
ModulationData AudioSystem::modData;

//--------------------------------------------------------------
AudioSystem::AudioSystem()
{
	modData.MockData();

	// initialise timers
	releaseTimer.setFunctionToCall(stopRiser);
	timeModulationTimer.setFunctionToCall(timeModulation);

	/* modulation improvements
	* curve per modulator
	* seek speed per modulator
	* range setting
	* range per modulator
	*/

	// timer with threshold of start to decrease intensity
	// after this timer goes of, a new timer should start decreasing the intensity
	/* required features: (seperate class)
	* init function that starts the timer based on the length
	* only start after this timer went off
	* input for after how long this hould happen
	* curve by which this should happen
	* seek speed how fast this should happen
	* optional: how much this should happen
	*/

	// intialise vector to check if movement is less
	/* required features: (seperate class)
	* init function that fills an empty vector
	* check function to check if should be decreasing
	* input for after how long this should happen
	* curve
	* seek speed
	* optional: how much
	*/
	vector<float> vect(40, 0.f);
	lastValues = vect;
}

AudioSystem::~AudioSystem()
{
	for (auto layer : layerLoops) {
		delete layer;
	}
	layerLoops.clear();
	for (auto layer : layerImpacts) {
		delete layer;
	}
	layerImpacts.clear();
}

//--------------------------------------------------------------
// audio loading and initialising
//--------------------------------------------------------------
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
	}
}

//--------------------------------------------------------------
// update
//--------------------------------------------------------------

void AudioSystem::update() {
	FMOD_System_Update(sys);

	if (playing) {
		// get the players position in float
		float decimalValue = setDecimalValue(modData);

		// attack envelope
		float attackedGain = attackEnv.arAttackExp(_gain, envelopeTrigger);

		// plotting
		int onLayers = 0;
		mainOutputGainAllLayers = 0;
		mainFrequencyAllLayers = 0;

		for (auto layer : layerLoops) {
			if (layer->_onOff) {

				// gain modulation
				float outputGain = attackedGain * layer->gainModulation(decimalValue, modulationTrigger, timeModulationTrigger);
				layer->setVolume(outputGain);

				// pitch modulation
				if (layer->mainPitchModToggle) {
					float outputPitch = (layer->frequencyRange * frequencyStandard) * layer->pitchModulation(decimalValue, modulationTrigger, timeModulationTrigger); 
					layer->setFrequency(outputPitch);
				}

				// get main amplitude and pitch (used for plotting)
				onLayers++;
				mainOutputGainAllLayers += attackedGain * outputGain;
				mainFrequencyAllLayers += layer->getFrequency();
			}
		}

		// set main amplitude and pitch (used for plotting)
		mainOutputGainAllLayers /= onLayers;
		mainFrequencyAllLayers /= onLayers;

		// update timers
		releaseTimer.update();
		timeModulationTimer.update();

		// reset trigger for envelopes
		if (envelopeTrigger == 1) envelopeTrigger = 0;
	}
}

//--------------------------------------------------------------
// playback
//--------------------------------------------------------------

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

	timeModulationTimer.startTimer();

	FMOD_ChannelGroup_SetVolume(channelgroup, 1);
	startAudioLayers(layerLoops);
}

// begin release phase of riser, with timer that checks when to completely stop the riser
void AudioSystem::startRelease() {
	debugMessage("start stopping audio");

	// reset triggers
	modulationTrigger = 0;
	timeModulationTrigger = 0;

	releaseTimer.startTimer();

	// play impacts
	startAudioLayers(layerImpacts);
}

void AudioSystem::stopRiser()
{
	debugMessage("stop riser");

	stopAudioLayers(layerLoops);
	stopAudioLayers(layerImpacts);
	FMOD_ChannelGroup_SetVolume(channelgroup, 0);

	// reset timers in case they are currently running
	releaseTimer.stopTimer();
	timeModulationTimer.stopTimer();

	playing = false;
}

void AudioSystem::startAudioLayers(vector<LoopLayer*> layersToStart) {
	debugMessage("start Audio loops");
	for (auto layer : layersToStart) {
		if (layer->_onOff) {
			layer->startSounds();
		}
	}
}

void AudioSystem::startAudioLayers(vector<ImpactLayer*> layersToStart) {
	debugMessage("start Audio impacts");
	for (auto layer : layersToStart) {
		layer->startSounds();
	}
}

void AudioSystem::stopAudioLayers(vector<LoopLayer*> layersToStop) {
	debugMessage("stop Audio loops");
	for (auto layer : layersToStop) {
		layer->stopSounds();
	}
}

void AudioSystem::stopAudioLayers(vector<ImpactLayer*> layersToStop) {
	debugMessage("stop Audio impacts");
	for (auto layer : layersToStop) {
		layer->stopSounds();
	}
}

//--------------------------------------------------------------
// setters
//--------------------------------------------------------------

void AudioSystem::setGain(float gain) {
	debugMessage("setGain: " + to_string(gain));
	_gain = pow(10, gain / 20);
}

void AudioSystem::setPositionGainModulation(float attack)
{
	debugMessage("setPositionModulation: Amp. " + to_string(attack));
	for (auto layer : layerLoops) {
		layer->positionGainMod.CalculateAttackStepSize(attack, attack * 1.5);
	}
}

void AudioSystem::setPositionPitchModulation(float attack)
{
	debugMessage("setPositionModulation: Pitch. " + to_string(attack));
	for (auto layer : layerLoops) {
		if (layer->mainPitchModToggle) { layer->positionPitchMod.CalculateAttackStepSize(attack, attack * 1.5); }
	}
}

void AudioSystem::setAttack(float attack) {
	debugMessage("setAttack: " + to_string(attack));
	attackEnv.setARExp(attack, 0);
}

void AudioSystem::setRelease(float release)
{
	debugMessage("setRelease: " + to_string(release));

	for (auto layer : layerLoops) {
		layer->positionGainMod.CalculateReleaseStepSize(release);
	}

	for (auto layer : layerLoops) {
		if (layer->mainPitchModToggle) { layer->positionPitchMod.CalculateReleaseStepSize(release); }
	}

	// set timer length
	releaseTimer.setLength(release + 50);
}

void AudioSystem::setModulationCurve(float startValue)
{
	debugMessage("setcurve: " + to_string(startValue));
	for (auto layer : layerLoops) {
		layer->positionGainMod.amplitudeStartValue = startValue;
	}
}

void AudioSystem::setPosition(float position)
{
	modData.currentDistanceToGetTo = position;
}

void AudioSystem::setTimeModulationThreshold(float modulation)
{
	debugMessage("setTimeModulationThreshold: " + to_string(modulation));
	timeModulationTimer.setLength(modulation);
}

void AudioSystem::setTimeModulationLength(float lengthInMs)
{
	debugMessage("setTimeModulationLength " + to_string(lengthInMs));
	for (auto layer : layerLoops) {
		layer->timeGainMod.CalculateStepSize(lengthInMs);
	}
}

//--------------------------------------------------------------
// getters
//--------------------------------------------------------------

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

//--------------------------------------------------------------
// time modulation
//--------------------------------------------------------------

// after how long should the riser slowdown when goal hasnt been reached yet
// generally this point should not be reached as action over time is checked by the action timer, but due to players maybe going afk etc, you might want the timer to slow down after a long amount of time
void AudioSystem::setTimer(float slowdownTimeMs, float slowDownAmount) 
{
}

void AudioSystem::timeModulation() 
{
	debugMessage("start time modulation");
	timeModulationTrigger = 1;
}

//--------------------------------------------------------------
// action modulation
//--------------------------------------------------------------

// after how long and how much defiation in player position should the riser slow down
void AudioSystem::checkLessModifier(float value) {
	// check every 10 ticks to set lessGain
	if (currentTick == 10) {
		// use setcurrentdistance value to convert the value to a 0 1 value in the engine (if this is not done before)
		//float currentDistanceValue = setCurrentDistanceValue(decimalValue, mainOutputGainAllLayers);

		checkLessModifier(value);

		currentTick = 0;
	}
	else {
		currentTick++;
	}


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

	//if (currentDeviation - value <= threshold && currentDeviation - value >= 0 - threshold) {
	//	debugMessage("current deviation" + to_string(currentDeviation) = " | value: " + to_string(value));
	//}
	// check only for if its bigger
	// then check if has not changed enough
}

//--------------------------------------------------------------
// Offset
//--------------------------------------------------------------

void AudioSystem::setOffset(float offset) {
	// check if offset is necessary
	// check if max offset is necessary (maybe only needs half of the time, as the sound does need to get more intense)
	// set values for offset to happen and sound to modulate to its conclusion
}


