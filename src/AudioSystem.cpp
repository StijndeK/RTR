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
	//Modulation mod1;
	//Modulation mod2;
	//mod1.modType = exponential;
	//mod2.modType = exponential;
	//modData.currentDistanceToGetTo = 1;

	//mod1.CalculateStepSize(1000, 1000, 500);
	//mod2.CalculateStepSize(1000, 1000, 500);
	//float out;
	//float out2;
	//for (int i = 0; i < 30; i++) {
	//	out = mod1.CalculateModulation(modData.currentDistanceToGetTo, 1);
	//	out2 = mod2.CalculateModulation(out, 1);
	//}

	////float out = mod1.CalculateModulation(modData.currentDistanceToGetTo, 1);
	//cout << out << endl;
	////float out2 = mod2.CalculateModulation(out, 1);
	//cout << out2 << endl;

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

	// this value to get to in the audio == the player position scaled to a value between 0 and 1
	// float decimalValue = modData.ConvertToDecimalData(); 	// get value to get to from distance in float to value between 0 and 1 from distance values
	// in mock no need to convert, use the currentPositionSlider value (which is already between 0 and 1)
	float decimalValue = modData.currentDistanceToGetTo;

	// attack envelope
	float attackedGain = attackEnv.arAttackExp(_gain, envelopeTrigger);

	// modulate gain audio
	float outputGain = mainGainModulation.CalculateModulation(decimalValue, trigger);

	for (auto layer : layerLoops) {
		if (layer->_onOff) {
			float layerOutputGain = layer->gainMod.CalculateModulation(decimalValue, trigger);
			//float layerOutputGain = outputGain;
			FMOD_Channel_SetVolume(layer->_channel, layerOutputGain * attackedGain * outputGain);
			debugMessage(layer->_label + to_string(layerOutputGain * attackedGain * outputGain));
		}
	}

	//debugMessage(to_string(outputGain));

	// modulate pitch
	float frequencyRange = 1.5;
	float outputPitch = mainPitchModulation.CalculateModulation(decimalValue, trigger);
	for (auto layer : pitchModLayers) {
		if (layer->_onOff) {
			float layerOutputPitch = layer->pitchMod.CalculateModulation(outputPitch, 1);
			FMOD_Channel_SetFrequency(layer->_channel, layerOutputPitch * (frequencyRange * frequencyStandard));
		}
	}
	
	// reset trigger
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
	}
}

void AudioSystem::playAudioLoops() {
	stopAudio(layerLoops);

	// create snapshot of gain for envelopes
	gainSnapshot = _gain;
	// trigger envelopes and modulation
	trigger = 1;
	envelopeTrigger = 1;

	debugMessage("Now Playing Loops: ");
	for (auto layer : layerLoops) {
		// check if layer is on
		if (layer->_onOff) {
			FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, layer->_sounds[layer->_currentSoundIdentifier], false, &layer->_channel);
			debugMessage("	" + layer->_label + ":");
			debugMessage("		" + getAudioName(layer->_sounds[layer->_currentSoundIdentifier]));
		}
	}

	// FMOD_Channel_SetVolume(getLayerByName("Noise")->_channel, 0.2);
}

void AudioSystem::playAudioImpacts() {
	debugMessage("Now Playing Impacts: ");
	for (auto layer : layerImpacts) {
		int randValue = rand() % 3; 	// randomise sub and impact selection
		FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, layer->_sounds[randValue], false, &layer->_channel);
		debugMessage(getAudioName(layer->_sounds[randValue]));
	}
}

void AudioSystem::playEnvelopes()
{
	//// attack
	//float attackedGain = attackEnv.arAttackExp(_gain, trigger);
	////debugMessage("attackedGain: " + to_string(attackedGain));

	//// amplitude modulation
	//for (auto layer : layerLoops) {
	//	if (layer->_onOff) {
	//		float envelopeGain = layer->gainEnv.arExp(_gain, trigger);
	//		FMOD_Channel_SetVolume(layer->_channel, envelopeGain);
	//		//debugMessage("gain: " + to_string(envelopeGain));
	//	}
	//}

	//// pitch modulation
	//for (auto layer : pitchModLayers) {
	//	if (layer->_onOff) {
	//		float envelopePitch = layer->pitchEnv.arLin(1.5 * frequencyStandard, trigger);
	//		FMOD_Channel_SetFrequency(layer->_channel, envelopePitch);
	//		//debugMessage("pitch: " + to_string(envelopePitch));
	//	}
	//}

	//// play impact at the peak
	//float impactTrigger = layerImpacts[0]->gainEnv.arExp(1, trigger);
	//if (layerImpacts[0]->gainEnv.currentEnvState == 1) {
	//	debugMessage("impact");
	//	playAudioImpacts();
	//}
}

void AudioSystem::startStopping() {
	debugMessage("start stopping audio");
	trigger = 0;
	playAudioImpacts();

	// check if trigger needs to be set to 0 immediatly, or if there is an offset: if the riser can still rise a bit. and iddeally update the speed of the riser to reach the full end for the impact

	// after release is done completely stop the sounds
}

void AudioSystem::stopAudio(vector<Layer*> layersToStop) {
	debugMessage("stop Audio");
	for (auto layer : layersToStop) {
		FMOD_Channel_Stop(layer->_channel);
	}
}

void AudioSystem::setGain(float gain) {
	debugMessage("setGain: " + to_string(gain));
	_gain = pow(10, gain / 20);
}

void AudioSystem::setPan(float p) 
{
	//p = ofClamp(p, -1, 1);
	//FMOD_Channel_SetPan(channel, p);
	//FMOD_System_Update(sys);
}

void AudioSystem::setEnvelopes(modulationParameter type, float attack, float range, float curve)
{
	//if (type == Amp) {
	//	for (auto layer : layerLoops) {
	//		layer->gainEnv.setARExp(attack, 500); // use default release value, to be replaced with new release slider input
	//	}
	//	layerImpacts[0]->gainEnv.setARExp(attack, 500);
	//}
	//else if (type == Pitch) {
	//	for (auto layer : pitchModLayers) {
	//		layer->pitchEnv.setARLin(attack, 500);
	//		layer->pitchModulationRange = range;
	//		// TODO amp start value based on curve
	//	}
	//}
}

void AudioSystem::setModulation(modulationParameter type, float attack, float range, float curve)
{
	if (type == Amp) {
		debugMessage("setModulation: Amp. " + to_string(attack));
		mainGainModulation.CalculateStepSize(attack, attack * 1.5, 2000);
		for (auto layer : layerLoops) {
			layer->gainMod.CalculateStepSize(attack, attack * 1.5, 2000);
		}
	}
	else if (type == Pitch) {
		debugMessage("setModulation: Pitch. " + to_string(attack));
		mainPitchModulation.CalculateStepSize(attack, attack * 1.5, 2000);
		for (auto layer : pitchModLayers) {
			layer->pitchModulationRange = range;
			layer->pitchMod.CalculateStepSize(attack, attack * 1.5, 2000);
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