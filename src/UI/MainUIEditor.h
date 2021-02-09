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
    void update();

    void initGui(ofxDatGui* gui, string headerName, int theme = 0);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void initialiseAllValues();
    
private:
    ofApp* _ofApp;

    ofxDatGuiValuePlotter* gainPlotter;
    ofxDatGuiWaveMonitor* waveMonitor;
    ofxDatGuiToggle* padStartToggle;
    ofxDatGuiToggle* padEndToggle;
    ofxDatGuiToggle* fxToggle;
    ofxDatGuiToggle* noiseToggle;
    ofxDatGuiToggle* shepardsToggle;
};

