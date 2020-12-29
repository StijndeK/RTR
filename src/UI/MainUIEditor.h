#pragma once
#include "ofMain.h"
 #include "JSON/JsonSystem.h"
 #include "TestUIEditor.h"
#include "ofxDatGui.h"
class ofApp;

class MainUIEditor {
public:
    MainUIEditor(ofApp* appReference);
    ~MainUIEditor();

    void setup();
    void draw();
    void keyPressed(int key);

    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onButtonEvent(ofxDatGuiButtonEvent e);

    void setAudioValues();
    
    TestUIEditor testUI;

    float tGain;
    float tPan;

    ofApp* _ofApp;
};

