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

    // general gui
    ofxDatGui* gui = new ofxDatGui(100, 100);
    gui->onSliderEvent(this, &MainUIEditor::onSliderEvent);
    gui->onButtonEvent(this, &MainUIEditor::onButtonEvent);

    gui->addHeader(":: Drag Me To Reposition ::");
    gui->addFooter();

    // gain slider
    ofxDatGuiSlider* gainSlider = gui->addSlider("gain", 0, 1);
    gainSlider->bind(tGain);
    gainSlider->setValue(_ofApp->jsonSys.getValue("gain")); // init with saved value

    // pan slider
    ofxDatGuiSlider* panSlider = gui->addSlider("pan", -1, 1);
    gainSlider->bind(tPan);
    gainSlider->setValue(_ofApp->jsonSys.getValue("pan")); // init with saved value

    // set button
    gui->addButton("Set");
}

void MainUIEditor::draw() {
    ofBackground(0);
    ofSetHexColor(0x00FF00);
    _ofApp->jsonSys.draw();
}

void MainUIEditor::onSliderEvent(ofxDatGuiSliderEvent e)
{
    _ofApp->jsonSys.setValue(e.target->getLabel(), e.target->getValue());
    setAudioValues();
}

void MainUIEditor::onButtonEvent(ofxDatGuiButtonEvent e)
{
    setAudioValues();
    // testUI.onButtonEvent(e);
    cout << e.target->getLabel() << endl; // prints "My Button"
}

void MainUIEditor::setAudioValues()
{
    _ofApp->audio.setGain(_ofApp->jsonSys.getValue("gain"));
    _ofApp->audio.setPan(_ofApp->jsonSys.getValue("pan"));
}

void MainUIEditor::keyPressed(int key) {
    _ofApp->audio.playAudio();
}

