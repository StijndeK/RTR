#include "MainUIEditor.h"
#include "ofApp.h"

MainUIEditor::MainUIEditor(ofApp* appReference)
{
    _ofApp = appReference;
}

MainUIEditor::~MainUIEditor()
{
}

void MainUIEditor::setup() {
    _ofApp->audio.initFMODSystem();

    // TODO: skew factors for sliders

    // GUI
    int offset = 5;

    // general gui
    ofxDatGui* guiGeneral = new ofxDatGui(0 + offset, 0 + offset);
    initGui(guiGeneral);
    guiGeneral->addHeader(":: General Editor ::", false);

    ofxDatGuiSlider* gainSlider = guiGeneral->addSlider("gain", -90, 0, _ofApp->jsonSys.getValue("gain")); // init with saved value
    // gainSlider->bind(_ofApp->audio._gain);
    ofxDatGuiSlider* offsetSlider = guiGeneral->addSlider("offset", 0, 2000, _ofApp->jsonSys.getValue("offset")); // init with saved value
    ofxDatGuiSlider* attackSlider = guiGeneral->addSlider("attack", 200, 5000, _ofApp->jsonSys.getValue("attack")); // init with saved value
    attackSlider->setPrecision(0);
    guiGeneral->addToggle("Fullscreen", false);
    guiGeneral->addButton("Select destination");

    // mock gui
    ofxDatGui* guiMock = new ofxDatGui(255 + offset, 0 + offset);
    initGui(guiMock);
    guiMock->addHeader(":: Mock Editor ::", false);

    guiMock->addButton("Play");
    guiMock->addButton("Stop");
    ofxDatGuiSlider* rangeSlider = guiMock->addSlider("range in ms", 5000, 20000, _ofApp->jsonSys.getValue("range in ms"));  // init with saved value
    rangeSlider->setPrecision(0);

    // sound gui
    ofxDatGui* guiSound = new ofxDatGui(510 + offset, 0 + offset);
    initGui(guiSound);
    guiSound->addHeader(":: Sound Editor ::", false);
    guiSound->addToggle("Pad: Start");
    guiSound->addToggle("Pad: End");
    guiSound->addToggle("Fx");
    guiSound->addToggle("Noise");
    guiSound->addToggle("Shepards");

    // initialise audio values
    setAudioValue();
}

void MainUIEditor::draw() {
    ofBackground(ofColor(0, 0, 0));
    ofSetHexColor(0x00FF00);

    _ofApp->jsonSys.draw();
}

void MainUIEditor::initGui(ofxDatGui* gui) {
    gui->setTheme(new ofxDatGuiThemeSmoke());
    gui->setWidth(250);
    gui->onSliderEvent(this, &MainUIEditor::onSliderEvent);
    gui->onButtonEvent(this, &MainUIEditor::onButtonEvent);
}

void MainUIEditor::onSliderEvent(ofxDatGuiSliderEvent e)
{
    _ofApp->jsonSys.setValue(e.target->getLabel(), e.target->getValue());
    setAudioValue();
}

void MainUIEditor::onButtonEvent(ofxDatGuiButtonEvent e)
{
    string label = e.target->getLabel();
    if (label == "Play") {
        _ofApp->audio.playAudioLoops();
    }
    else if (label == "Stop") {
        _ofApp->audio.stopAudio(_ofApp->audio.layerLoops);
    }
    else if (label == "Fullscreen") {
        ofToggleFullscreen();
    }
    else if (label == "Select destination") {
        _ofApp->jsonSys.getPath();
    }
    else { // set layer on or off TODO: create seperate function
        _ofApp->audio.getLayerByName(label)->_onOff = !_ofApp->audio.getLayerByName(label)->_onOff;
    }
    //else {
    //    ofLogError("MainUIEditor::onButtonEvent") << "unknown label type: " << e.target->getLabel() << endl;
    //}
}

void MainUIEditor::setAudioValue()
{
    _ofApp->audio.setGain(_ofApp->jsonSys.getValue("gain"));
    _ofApp->audio.setEnvelopes(_ofApp->audio.Amp, _ofApp->jsonSys.getValue("range in ms"), 1, 0);
    _ofApp->audio.setEnvelopes(_ofApp->audio.Pitch, _ofApp->jsonSys.getValue("range in ms"), 1, 0);
    _ofApp->audio.setAttack(_ofApp->jsonSys.getValue("attack"));
    _ofApp->audio.setOffset(_ofApp->jsonSys.getValue("offset"));
}
