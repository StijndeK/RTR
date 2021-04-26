#pragma once
#include "ofMain.h"
#include "JSON/JsonInOut.h"
#include "ofxDatGui.h"
class RTRStudio;

class MainUIEditor {
public:
    MainUIEditor(RTRStudio* appReference);

    // Of functions
    void setup();
    void draw();
    void update();

    void initGui(ofxDatGui* gui, string headerName, int theme = 0);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void getProjectLocation();
    void initialiseAllValues();
    
private:
    RTRStudio* _ofApp;
    ofxDatGuiValuePlotter* gainPlotter;
    ofxDatGuiWaveMonitor* waveMonitor;
    ofxDatGuiToggle* padStartToggle;
    ofxDatGuiToggle* padEndToggle;
    ofxDatGuiToggle* fxToggle;
    ofxDatGuiToggle* noiseToggle;
    ofxDatGuiToggle* shepardsToggle;
};

