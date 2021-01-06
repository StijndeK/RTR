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
    ofxDatGuiSlider* offsetSlider = guiGeneral->addSlider("offset", 0, 1, _ofApp->jsonSys.getValue("offset")); // init with saved value
    ofxDatGuiSlider* attackSlider = guiGeneral->addSlider("attack", 200, 5000, _ofApp->jsonSys.getValue("attack")); // init with saved value
    attackSlider->setPrecision(0);
    guiGeneral->addToggle("Fullscreen", false);

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
    guiSound->addToggle("Pad");

    // initialise audio values
    setAudioValue(); 
}

void MainUIEditor::draw() {
    ofBackground(90, 90, 93);
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
    if (e.target->getLabel() == "Play") {
        _ofApp->audio.playAudioLoops();
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
    _ofApp->audio.setAttack(_ofApp->jsonSys.getValue("attack"));
}
