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
    testUI.setup();

    int offset = 25;

    // general gui
    ofxDatGui* guGeneral = new ofxDatGui(0 + offset, 0 + offset);
    guGeneral->setWidth(250);
    guGeneral->onSliderEvent(this, &MainUIEditor::onSliderEvent);
    guGeneral->onButtonEvent(this, &MainUIEditor::onButtonEvent);
    guGeneral->addHeader(":: General Editor ::", false);

    ofxDatGuiSlider* gainSlider = guGeneral->addSlider("gain", 0, 1); // init with saved value
    gainSlider->setValue(_ofApp->jsonSys.getValue("gain")); 

    // mock gui
    ofxDatGui* guiMock = new ofxDatGui(300 + offset, 0 + offset);
    guiMock->onSliderEvent(this, &MainUIEditor::onSliderEvent);
    guiMock->onButtonEvent(this, &MainUIEditor::onButtonEvent);
    guiMock->setWidth(250);
    guiMock->addHeader(":: Mock Editor ::", false);

    guiMock->addButton("Play");
    guiMock->addButton("Stop");

    // sound gui
    ofxDatGui* guiSound = new ofxDatGui(600 + offset, 0 + offset);
    guiSound->onSliderEvent(this, &MainUIEditor::onSliderEvent);
    guiSound->onButtonEvent(this, &MainUIEditor::onButtonEvent);
    guiSound->setWidth(250);
    guiSound->addHeader(":: Sound Editor ::", false);
}

void MainUIEditor::draw() {
    ofBackground(90, 90, 93);
    ofSetHexColor(0x00FF00);
    _ofApp->jsonSys.draw();
}

void MainUIEditor::onSliderEvent(ofxDatGuiSliderEvent e)
{
    cout << e.target->getLabel() << endl; // prints "My Button"
    _ofApp->jsonSys.setValue(e.target->getLabel(), e.target->getValue());
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
    else {
        ofLogError("MainUIEditor::onButtonEvent") << "unknown label type: " << e.target->getLabel() << endl;
    }
}

void MainUIEditor::setAudioValue()
{
    _ofApp->audio.setGain(_ofApp->jsonSys.getValue("gain"));
}

void MainUIEditor::keyPressed(int key) {
    _ofApp->audio.playAudio();
}

