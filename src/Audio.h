#pragma once

#include "ofMain.h"
#include "fmod.h"

class Audio {

public:
	void setup();
	void update();
	void load();
	void play();

	void initFMOD();

	FMOD_SOUND* sound;
	FMOD_CHANNEL* channel;
};
