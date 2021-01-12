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

    // sound gui
    ofxDatGui* guiSound = new ofxDatGui(0 + offset, 0 + offset);
    initGui(guiSound);
    guiSound->onToggleEvent(this, &MainUIEditor::onToggleEvent);
    guiSound->addHeader(":: Sound Editor ::", false);
    // set the defaults from json
    padStartToggle = guiSound->addToggle("Pad: Start", _ofApp->jsonSys.getValue("Pad: Start"));
    padEndToggle = guiSound->addToggle("Pad: End", _ofApp->jsonSys.getValue("Pad: End"));
    fxToggle = guiSound->addToggle("Fx", _ofApp->jsonSys.getValue("Fx"));
    noiseToggle = guiSound->addToggle("Noise", _ofApp->jsonSys.getValue("Noise"));
    shepardsToggle = guiSound->addToggle("Shepards", _ofApp->jsonSys.getValue("Shepards"));

    // general gui
    ofxDatGui* guiGeneral = new ofxDatGui(255 + offset, 0 + offset);
    initGui(guiGeneral);
    guiGeneral->addHeader(":: General Editor ::", false);

    ofxDatGuiSlider* gainSlider = guiGeneral->addSlider("gain", -90, 0, _ofApp->jsonSys.getValue("gain")); // init with saved value
    ofxDatGuiSlider* offsetSlider = guiGeneral->addSlider("offset", 0, 2000, _ofApp->jsonSys.getValue("offset")); // init with saved value
    ofxDatGuiSlider* attackSlider = guiGeneral->addSlider("attack", 200, 5000, _ofApp->jsonSys.getValue("attack")); // init with saved value
    attackSlider->setPrecision(0);
    guiGeneral->addToggle("Fullscreen", false);
    guiGeneral->addButton("Select destination");

    // mock gui
    ofxDatGui* guiMock = new ofxDatGui(510 + offset, 0 + offset);
    initGui(guiMock);
    guiMock->addHeader(":: Mock Editor ::", false);

    guiMock->addButton("Start");
    guiMock->addButton("Impact");
    ofxDatGuiSlider* rangeSlider = guiMock->addSlider("range in ms", 5000, 20000, _ofApp->jsonSys.getValue("range in ms"));  // init with saved value
    rangeSlider->setPrecision(0);
    ofxDatGuiSlider* currentPositionSlider = guiMock->addSlider("currentPosition", 0, 1, _ofApp->jsonSys.getValue("currentPosition"));  // init with saved value
    currentPositionSlider->bind(_ofApp->audio.modData.currentDistanceToGetTo);

    gainPlotter = guiMock->addValuePlotter("Gain", 0, 1);
    gainPlotter->setValue(0);
    gainPlotter->setSpeed(0.5);

    waveMonitor = guiMock->addWaveMonitor("wave", 0, 0);
    waveMonitor->setDrawMode(ofxDatGuiGraph::LINES);

    // visualisation gui
    //ofxDatGui* visualMock = new ofxDatGui(100, guiSound->getHeight() + 20);
    //visualMock->setTheme(new ofxDatGuiThemeSmoke());
    //visualMock->setWidth(250);

    //ofxDatGui* visualMock2 = new ofxDatGui(450, guiSound->getHeight() + 20);
    //visualMock2->setTheme(new ofxDatGuiThemeSmoke());
    //visualMock2->setWidth(250);

    // initialise audio values
    initialiseAllValues();
}

void MainUIEditor::draw() {
    ofBackground(ofColor(0, 0, 0));
    ofSetHexColor(0x00FF00);

    _ofApp->jsonSys.draw();
}

void MainUIEditor::update() {
    // plotters
    gainPlotter->setValue(_ofApp->audio.mainOutputGainAllLayers);
    waveMonitor->setAmplitude(_ofApp->audio.mainOutputGainAllLayers);
    waveMonitor->setFrequency(_ofApp->audio.mainFrequencyAllLayers);
}

void MainUIEditor::initGui(ofxDatGui* gui) {
    gui->setTheme(new ofxDatGuiThemeSmoke());
    gui->setWidth(250);
    gui->onSliderEvent(this, &MainUIEditor::onSliderEvent);
    gui->onButtonEvent(this, &MainUIEditor::onButtonEvent);
}

void MainUIEditor::onSliderEvent(ofxDatGuiSliderEvent e)
{
    // export to json
    _ofApp->jsonSys.setValue(e.target->getLabel(), e.target->getValue());

    string label = e.target->getLabel();

    //setAudioValue();
    if (label == "gain") {
        _ofApp->audio.setGain(_ofApp->jsonSys.getValue(label));
    }
    else if (label == "attack") {
        _ofApp->audio.setAttack(_ofApp->jsonSys.getValue(label));
    }
    else if (label == "range in ms") {
        _ofApp->audio.setGainModulation(_ofApp->jsonSys.getValue(label));
        _ofApp->audio.setPitchModulation(_ofApp->jsonSys.getValue(label));
    }
    else if (label == "offset") {
        _ofApp->audio.setOffset(_ofApp->jsonSys.getValue("offset"));
    }
    else {
        cout << "Error: slider label not found" << endl;
    }
}

// TODO: do data conversion within the UI class
void MainUIEditor::onButtonEvent(ofxDatGuiButtonEvent e)
{
    string label = e.target->getLabel();

    if (label == "Start") {
        _ofApp->audio.startRiser();
    }
    else if (label == "Impact") {
        _ofApp->audio.startRelease();
    }
    else if (label == "Fullscreen") {
        ofToggleFullscreen();
    }
    else if (label == "Select destination") {
        _ofApp->jsonSys.getPath();
    }
    else { 
        cout << "Error: button label not found" << endl;
    }
}

void MainUIEditor::onToggleEvent(ofxDatGuiToggleEvent e)
{
    string label = e.target->getLabel();

    // export to json
    _ofApp->jsonSys.setValue(e.target->getLabel(), e.target->getChecked());

    _ofApp->audio.getLayerByName(label)->_onOff = e.target->getChecked();
    _ofApp->audio.debugMessage("onoff for label: " + label + to_string(_ofApp->audio.getLayerByName(label)->_onOff));

    // NOTE: only toggles for layers exist, so no need to check which is pressed
}

void MainUIEditor::initialiseAllValues()
{
    _ofApp->audio.setGain(_ofApp->jsonSys.getValue("gain"));
    _ofApp->audio.setGainModulation(_ofApp->jsonSys.getValue("range in ms"));
    _ofApp->audio.setPitchModulation(_ofApp->jsonSys.getValue("range in ms"));
    _ofApp->audio.setAttack(_ofApp->jsonSys.getValue("attack"));
    _ofApp->audio.setOffset(_ofApp->jsonSys.getValue("offset"));

    onToggleEvent(padStartToggle);
    onToggleEvent(padEndToggle);
    onToggleEvent(fxToggle);
    onToggleEvent(noiseToggle);
    onToggleEvent(shepardsToggle);
}
