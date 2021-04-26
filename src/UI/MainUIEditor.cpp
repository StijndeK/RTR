#include "MainUIEditor.h"
#include "ofApp.h"

MainUIEditor::MainUIEditor(RTRStudio* appReference)
{
    _ofApp = appReference;
}

void MainUIEditor::setup() 
{
    _ofApp->audio.initFMODSystem();

    // GUI
    int offset = 5;

    // Title gui
    ofxDatGui* guiTitle = new ofxDatGui(10 + offset, 45 + offset);
    guiTitle->setTheme(new ofxDatGuiThemeSmoke());
    guiTitle->setWidth(110);
    guiTitle->addToggle("Fullscreen", false);
   
    ofxDatGui* guiTitle2 = new ofxDatGui(130 + offset, 45 + offset);
    guiTitle2->setTheme(new ofxDatGuiThemeSmoke());
    guiTitle2->setWidth(110);
    guiTitle2->addButton("Project location");
    guiTitle2->onButtonEvent(this, &MainUIEditor::onButtonEvent);

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

    // ROW 1
    float firstRowHeight = 80 + offset;

    // sound gui
    ofxDatGui* guiSound = new ofxDatGui(0 + offset, firstRowHeight);
    initGui(guiSound, ":: Sound ::");
    guiSound->onToggleEvent(this, &MainUIEditor::onToggleEvent);
    padStartToggle = guiSound->addToggle("Pad: Start", _ofApp->jsonSys.getValue("Pad: Start"));
    padEndToggle = guiSound->addToggle("Pad: End", _ofApp->jsonSys.getValue("Pad: End"));
    fxToggle = guiSound->addToggle("Fx", _ofApp->jsonSys.getValue("Fx"));
    noiseToggle = guiSound->addToggle("Noise", _ofApp->jsonSys.getValue("Noise"));
    shepardsToggle = guiSound->addToggle("Shepards", _ofApp->jsonSys.getValue("Shepards"));

    // general gui
    ofxDatGui* guiGeneral = new ofxDatGui(255 + offset, firstRowHeight);
    initGui(guiGeneral, ":: Adaption ::");
    ofxDatGuiSlider* gainSlider = guiGeneral->addSlider("gain", -90, 0, _ofApp->jsonSys.getValue("gain")); 
    ofxDatGuiSlider* rangeSlider = guiGeneral->addSlider("range in s", 5, 60, _ofApp->jsonSys.getValue("range in s"));
    //ofxDatGuiSlider* offsetSlider = guiGeneral->addSlider("offset", 0, 2, _ofApp->jsonSys.getValue("offset")); 
    ofxDatGuiSlider* attackSlider = guiGeneral->addSlider("attack", 0.5, 5, _ofApp->jsonSys.getValue("attack"));         
    ofxDatGuiSlider* releaseSlider = guiGeneral->addSlider("release", 2, 5, _ofApp->jsonSys.getValue("release"));     

    // mock gui
    ofxDatGui* guiMock = new ofxDatGui(510 + offset, firstRowHeight);
    initGui(guiMock, ":: Mock ::");
    guiMock->addButton("Start");
    guiMock->addButton("Impact");
    ofxDatGuiSlider* currentPositionSlider = guiMock->addSlider("Position", 0, 1, 0); 

    // ROW 2
    float secondRowHeight = guiSound->getHeight() + 84 + offset;

    // position modulation gui
    ofxDatGui* guiPositionModulation = new ofxDatGui(0 + offset, secondRowHeight);
    initGui(guiPositionModulation, ":: Position Modulation ::", 1);
    ofxDatGuiSlider* attackDecreaseModifierSlider = guiPositionModulation->addSlider("attack decrease modifier", 0.5, 2, _ofApp->jsonSys.getValue("attack decrease modifier"));         
    attackDecreaseModifierSlider->setPrecision(2);
    ofxDatGuiSlider* curveSlider = guiPositionModulation->addSlider("curve shape", 0, 1, _ofApp->jsonSys.getValue("curve shape"));

    // action modulation gui
    ofxDatGui* guiActionModulation = new ofxDatGui(255 + offset, secondRowHeight);
    initGui(guiActionModulation, ":: Action Modulation ::", 1);
    ofxDatGuiSlider* actionModulationThresholdSlider = guiActionModulation->addSlider("am threshold", 0.01, 0.1, _ofApp->jsonSys.getValue("am threshold"));
    ofxDatGuiSlider* actionModulationLengthSlider = guiActionModulation->addSlider("am length", 0.5, 5, _ofApp->jsonSys.getValue("am length"));

    // time modulation gui
    ofxDatGui* guiTimeModulation = new ofxDatGui(510 + offset, secondRowHeight);
    initGui(guiTimeModulation, ":: Time Modulation ::", 1);
    ofxDatGuiSlider* timeModulationThresholdSlider = guiTimeModulation->addSlider("tm threshold", 1, 20, _ofApp->jsonSys.getValue("tm threshold"));
    timeModulationThresholdSlider->setPrecision(1);
    ofxDatGuiSlider* timeModulationLengthSlider = guiTimeModulation->addSlider("tm length", 0.5, 5, _ofApp->jsonSys.getValue("tm length"));

    // initialise audio values
    initialiseAllValues();
}

void MainUIEditor::draw() {
    ofBackground(ofColor(141, 158, 171));
    ofSetColor(0, 0, 0);
    
    std::stringstream ss;
    ss  << "\n" << "REAL-TIME RISERS" << endl;
    ofDrawBitmapString(ss.str(), 20, 15);
}

void MainUIEditor::update() {
    gainPlotter->setValue(_ofApp->audio.mainOutputGainAllLayers);
    waveMonitor->setAmplitude(_ofApp->audio.mainOutputGainAllLayers);
    waveMonitor->setFrequency(_ofApp->audio.mainFrequencyAllLayers);
}

void MainUIEditor::initGui(ofxDatGui* gui, string headerName, int theme) {
    gui->setTheme(new ofxDatGuiThemeSmoke());
    gui->setWidth(250);
    gui->onSliderEvent(this, &MainUIEditor::onSliderEvent);
    gui->onButtonEvent(this, &MainUIEditor::onButtonEvent);
    gui->addHeader(headerName, false);
}

void MainUIEditor::onSliderEvent(ofxDatGuiSliderEvent e)
{
    string label = e.target->getLabel();

    _ofApp->jsonSys.setValue(e.target->getLabel(), e.target->getValue());

    if (label == "gain") 
    {
        _ofApp->audio.dbToFloat(_ofApp->jsonSys.getValue(label));
    }
    else if (label == "release") 
    {
        _ofApp->audio.setReleaseLayers(_ofApp->jsonSys.getValue(label));
    }
    else if (label == "attack") 
    {
        _ofApp->audio.setAttack(_ofApp->jsonSys.getValue(label));
    }
    else if (label == "attack decrease modifier") 
    {
        _ofApp->audio.setPositionModifier(_ofApp->jsonSys.getValue(label));
        _ofApp->audio.calculatePositionGainModulation(_ofApp->jsonSys.getValue("range in s"));
        _ofApp->audio.calculatePositionPitchModulation(_ofApp->jsonSys.getValue("range in s"));
    }
    else if (label == "range in s") 
    {
        _ofApp->audio.calculatePositionGainModulation(_ofApp->jsonSys.getValue(label));
        _ofApp->audio.calculatePositionPitchModulation(_ofApp->jsonSys.getValue(label));
    }
    else if (label == "offset") 
    {
        _ofApp->audio.setOffset(_ofApp->jsonSys.getValue(label));
    }
    else if (label == "curve shape") 
    {
        _ofApp->audio.setModulationCurveLayers(_ofApp->jsonSys.getValue(label));
    }
    else if (label == "Position") 
    {
        _ofApp->audio.setPosition(e.target->getValue());
    }
    else if (label == "tm threshold") 
    {
        _ofApp->audio.setTimeModulationThreshold(e.target->getValue(), _ofApp->jsonSys.getValue("range in s"));
    }
    else if (label == "tm length") 
    {
        _ofApp->audio.setTimeModulationLengthLayers(e.target->getValue(), _ofApp->jsonSys.getValue("range in s"));
    }
    else if (label == "am threshold") 
    {
        _ofApp->audio.setActionModulationThreshold(e.target->getValue());
    }
    else if (label == "am length") 
    {
        _ofApp->audio.setActionModulationLengthLayers(e.target->getValue(), _ofApp->jsonSys.getValue("range in s"));
    }
    else {
        cout << "Error: slider label not found: " << label << endl;
    }
}

// TODO: do data conversion within the UI class
void MainUIEditor::onButtonEvent(ofxDatGuiButtonEvent e)
{
    string label = e.target->getLabel();

    if (label == "Start") 
    {
        _ofApp->audio.startRiser();
    }
    else if (label == "Impact") 
    {
        _ofApp->audio.startRelease();
    }
    else if (label == "Fullscreen") 
    {
        ofToggleFullscreen();
    }
    else if (label == "Project location") 
    {
        getProjectLocation();
    }
    else { 
        cout << "Error: button label not found: " << label << endl;
    }
}

void MainUIEditor::onToggleEvent(ofxDatGuiToggleEvent e)
{
    string label = e.target->getLabel();

    // Export to json.
    _ofApp->jsonSys.setValue(e.target->getLabel(), e.target->getChecked());

    _ofApp->audio.getLayerByName(label)->onOff = e.target->getChecked();
    _ofApp->audio.debugMessage("onoff for label: " + label + to_string(_ofApp->audio.getLayerByName(label)->onOff));
}

void MainUIEditor::getProjectLocation()
{
    string file;
    ofFileDialogResult result = ofSystemLoadDialog("Load file");
    if (result.bSuccess) file = result.getPath();
    cout << file << endl;

    _ofApp->jsonSys.setProjectLocation("Project location", file);
    initialiseAllValues();
}

void MainUIEditor::initialiseAllValues() // TODO: do this automatically
{
    _ofApp->audio.dbToFloat(_ofApp->jsonSys.getValue("gain"));
    _ofApp->audio.setOffset(_ofApp->jsonSys.getValue("offset"));
    _ofApp->audio.setModulationCurveLayers(_ofApp->jsonSys.getValue("curve shape"));
    _ofApp->audio.setPositionModifier(_ofApp->jsonSys.getValue("attack decrease modifier"));
    _ofApp->audio.calculatePositionGainModulation(_ofApp->jsonSys.getValue("range in s"));
    _ofApp->audio.calculatePositionPitchModulation(_ofApp->jsonSys.getValue("range in s"));
    _ofApp->audio.setAttack(_ofApp->jsonSys.getValue("attack"));
    _ofApp->audio.setReleaseLayers(_ofApp->jsonSys.getValue("release"));
    _ofApp->audio.setTimeModulationThreshold(_ofApp->jsonSys.getValue("tm threshold"), _ofApp->jsonSys.getValue("range in s"));
    _ofApp->audio.setTimeModulationLengthLayers(_ofApp->jsonSys.getValue("tm length"), _ofApp->jsonSys.getValue("range in s"));
    _ofApp->audio.setActionModulationThreshold(_ofApp->jsonSys.getValue("am threshold"));
    _ofApp->audio.setActionModulationLengthLayers(_ofApp->jsonSys.getValue("am length"), _ofApp->jsonSys.getValue("range in s"));

    onToggleEvent(padStartToggle); 
    onToggleEvent(padEndToggle);
    onToggleEvent(fxToggle);
    onToggleEvent(noiseToggle);
    onToggleEvent(shepardsToggle);
}