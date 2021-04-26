#pragma once
#include "ofMain.h"
#include "UI/MainUIEditor.h"
#include "Audio/AudioSystem.h"
#include "JSON/JsonInOut.h"

class RTRStudio : public ofBaseApp {

public:
	RTRStudio();
	~RTRStudio();

	void setup();
	void onButtonEvent(ofxDatGuiButtonEvent e);
	void update();
	void draw();
	void keyPressed(int key);

    MainUIEditor* mainUIEdit;
	JsonInOut jsonSys;
	AudioSystem audio;
};
