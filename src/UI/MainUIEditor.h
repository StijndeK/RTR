#include "ofMain.h"
#include "JSON/JsonSystem.h"
#include "AudioSystem.h"
#include "TestUIEditor.h"

class MainUIEditor {
public:
    MainUIEditor();
    ~MainUIEditor();

    void setup();
    void draw();
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onButtonEvent(ofxDatGuiButtonEvent e);

    void setAudioValues();

    void keyPressed(int key);
    
    TestUIEditor testUI;
    AudioSystem audio;

    float tGain;

    JsonSystem jsonSys; // TODO: this should be stored in the JSON and read from JSON by audio system (make a convertion function in JSONExport that takes the type and its value)

    // link to the json system of ofapp so that i can call the set function
};

