#include "MainUIEditor.h"
#include "ofApp.h"

// TODO: skew factors for sliders

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

    // Title gui
    ofxDatGui* guiTitle = new ofxDatGui(0 + offset, 40 + offset);
    guiTitle->setWidth(100);
    guiTitle->addToggle("Fullscreen", false);
   
    ofxDatGui* guiTitle2 = new ofxDatGui(120 + offset, 40 + offset);
    guiTitle2->setWidth(120);
    guiTitle2->addButton("Project location");

    // Visualisation gui 1
    ofxDatGui* guiVis1 = new ofxDatGui(255 + offset, 5 + offset);
    guiVis1->setTheme(new ofxDatGuiThemeSmoke());
    guiVis1->setWidth(250);
    gainPlotter = guiVis1->addValuePlotter("Intensity", 0, 1);
    gainPlotter->setValue(0);
    gainPlotter->setSpeed(0.5);

    // Visualisation gui 2
    ofxDatGui* guiVis2 = new ofxDatGui(510 + offset, 5 + offset);
    guiVis2->setTheme(new ofxDatGuiThemeSmoke());
    guiVis2->setWidth(250);
    waveMonitor = guiVis2->addWaveMonitor("Waveform", 0, 0);
    waveMonitor->setDrawMode(ofxDatGuiGraph::LINES);

    // sound gui
    ofxDatGui* guiSound = new ofxDatGui(0 + offset, 80 + offset);
    initGui(guiSound);
    guiSound->onToggleEvent(this, &MainUIEditor::onToggleEvent);
    guiSound->addHeader(":: Sound ::", false);
    // set the defaults from json
    padStartToggle = guiSound->addToggle("Pad: Start", _ofApp->jsonSys.getValue("Pad: Start"));
    padEndToggle = guiSound->addToggle("Pad: End", _ofApp->jsonSys.getValue("Pad: End"));
    fxToggle = guiSound->addToggle("Fx", _ofApp->jsonSys.getValue("Fx"));
    noiseToggle = guiSound->addToggle("Noise", _ofApp->jsonSys.getValue("Noise"));
    shepardsToggle = guiSound->addToggle("Shepards", _ofApp->jsonSys.getValue("Shepards"));

    // general gui
    ofxDatGui* guiGeneral = new ofxDatGui(255 + offset, 80 + offset);
    initGui(guiGeneral);
    guiGeneral->addHeader(":: General ::", false);

    ofxDatGuiSlider* gainSlider = guiGeneral->addSlider("gain", -90, 0, _ofApp->jsonSys.getValue("gain")); 
    ofxDatGuiSlider* offsetSlider = guiGeneral->addSlider("offset", 0, 2000, _ofApp->jsonSys.getValue("offset")); 
    ofxDatGuiSlider* attackSlider = guiGeneral->addSlider("attack", 200, 5000, _ofApp->jsonSys.getValue("attack")); 
    ofxDatGuiSlider* releaseSlider = guiGeneral->addSlider("release", 50, 5000, _ofApp->jsonSys.getValue("release")); 
    ofxDatGuiSlider* curveSlider = guiGeneral->addSlider("curve", 0.001, 0.1, _ofApp->jsonSys.getValue("curve"));
    attackSlider->setPrecision(0);
    curveSlider->setPrecision(3);

    // mock gui
    ofxDatGui* guiMock = new ofxDatGui(510 + offset, 80 + offset);
    initGui(guiMock);
    guiMock->addHeader(":: Mock ::", false);

    guiMock->addButton("Start");
    guiMock->addButton("Impact");
    ofxDatGuiSlider* rangeSlider = guiMock->addSlider("range in ms", 5000, 20000, _ofApp->jsonSys.getValue("range in ms")); 
    rangeSlider->setPrecision(0);
    ofxDatGuiSlider* currentPositionSlider = guiMock->addSlider("Position", 0, 1, _ofApp->jsonSys.getValue("Position")); 
    currentPositionSlider->bind(_ofApp->audio.modData.currentDistanceToGetTo);

    // initialise audio values
    initialiseAllValues();
}

void MainUIEditor::draw() {
    ofBackground(ofColor(0, 0, 0));
    //ofSetHexColor(0x00FF00);
    ofSetColor(240, 249, 250);

    //_ofApp->jsonSys.draw();
    std::stringstream ss;
    ss  << "\n" << "REAL-TIME RISERS" << endl;
    ofDrawBitmapString(ss.str(), 10, 14);
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

    if (label == "gain") {
        _ofApp->audio.setGain(_ofApp->jsonSys.getValue(label));
    }
    else if (label == "attack") {
        _ofApp->audio.setAttack(_ofApp->jsonSys.getValue(label));
    }
    else if (label == "release") {
        _ofApp->audio.setRelease(_ofApp->jsonSys.getValue(label));
    }
    else if (label == "range in ms") {
        _ofApp->audio.setGainModulation(_ofApp->jsonSys.getValue(label));
        _ofApp->audio.setPitchModulation(_ofApp->jsonSys.getValue(label));
    }
    else if (label == "offset") {
        _ofApp->audio.setOffset(_ofApp->jsonSys.getValue(label));
    }
    else if (label == "curve") {
        _ofApp->audio.setModulationCurve(_ofApp->jsonSys.getValue(label));
    }
    else {
        cout << "Error: slider label not found: " << label << endl;
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
        cout << "Error: button label not found: " << label << endl;
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
    _ofApp->audio.setModulationCurve(_ofApp->jsonSys.getValue("curve"));

    onToggleEvent(padStartToggle);
    onToggleEvent(padEndToggle);
    onToggleEvent(fxToggle);
    onToggleEvent(noiseToggle);
    onToggleEvent(shepardsToggle);
}
