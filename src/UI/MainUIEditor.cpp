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

    // GUI
    int offset = 5;

    // general gui
    ofxDatGui* guiGeneral = new ofxDatGui(0 + offset, 0 + offset);
    guiGeneral->setTheme(new ofxDatGuiThemeSmoke());
    guiGeneral->setWidth(250);
    guiGeneral->onSliderEvent(this, &MainUIEditor::onSliderEvent);
    guiGeneral->onButtonEvent(this, &MainUIEditor::onButtonEvent);
    guiGeneral->addHeader(":: General Editor ::", false);

    // TODO: skew factor
    ofxDatGuiSlider* gainSlider = guiGeneral->addSlider("gain", -90, 0, _ofApp->jsonSys.getValue("gain")); // init with saved value

    ofxDatGuiSlider* offsetSlider = guiGeneral->addSlider("offset", 0, 1, _ofApp->jsonSys.getValue("offset")); // init with saved value

    ofxDatGuiSlider* attackSlider = guiGeneral->addSlider("attack", 0, 1, _ofApp->jsonSys.getValue("attack")); // init with saved value

    guiGeneral->addToggle("Fullscreen", false);

    // mock gui
    ofxDatGui* guiMock = new ofxDatGui(255 + offset, 0 + offset);
    guiMock->setTheme(new ofxDatGuiThemeSmoke());
    guiMock->setWidth(250);
    guiMock->onSliderEvent(this, &MainUIEditor::onSliderEvent);
    guiMock->onButtonEvent(this, &MainUIEditor::onButtonEvent);
    guiMock->addHeader(":: Mock Editor ::", false);

    guiMock->addButton("Play");
    guiMock->addButton("Stop");

    ofxDatGuiSlider* rangeSlider = guiMock->addSlider("range in ms", 500, 20000, _ofApp->jsonSys.getValue("range in ms"));  // init with saved value
    rangeSlider->setPrecision(0);

    // sound gui
    ofxDatGui* guiSound = new ofxDatGui(510 + offset, 0 + offset);
    guiSound->setTheme(new ofxDatGuiThemeSmoke());
    guiSound->setWidth(250);
    guiSound->onSliderEvent(this, &MainUIEditor::onSliderEvent);
    guiSound->onButtonEvent(this, &MainUIEditor::onButtonEvent);
    guiSound->addHeader(":: Sound Editor ::", false);
}

void MainUIEditor::draw() {
    ofBackground(90, 90, 93);
    ofSetHexColor(0x00FF00);

    _ofApp->jsonSys.draw();
}

void MainUIEditor::onSliderEvent(ofxDatGuiSliderEvent e)
{
    // check for gain because needs to be calculated to decimal number
    if (e.target->getLabel() == "gain") {
        float dBiNFloat = pow(10, e.target->getValue() / 20);
        _ofApp->jsonSys.setValue(e.target->getLabel(), dBiNFloat);
    }
    else {
        _ofApp->jsonSys.setValue(e.target->getLabel(), e.target->getValue());
    }
    setAudioValue();
}

void MainUIEditor::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if (e.target->getLabel() == "Play") {
        _ofApp->audio.playAudio();
    }
    else if (e.target->getLabel() == "Stop") {
        _ofApp->audio.stopAudio();
    }
    else if (e.target->getLabel() == "Fullscreen") {
        ofToggleFullscreen();
    }
    else {
        ofLogError("MainUIEditor::onButtonEvent") << "unknown label type: " << e.target->getLabel() << endl;
    }
}

void MainUIEditor::setAudioValue()
{
    _ofApp->audio.setGain(_ofApp->jsonSys.getValue("gain"));
    _ofApp->audio.setEnvelope(_ofApp->jsonSys.getValue("range in ms"));
}
