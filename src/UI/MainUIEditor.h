#pragma once
#include "ofMain.h"
#include "JSON/JsonSystem.h"
#include "ofxDatGui.h"
class ofApp;

class MainUIEditor {
public:
    MainUIEditor(ofApp* appReference);
    ~MainUIEditor();

    void setup();
    void draw();

    void initGui(ofxDatGui* gui);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void setAudioValue();
    
    ofApp* _ofApp;
};

