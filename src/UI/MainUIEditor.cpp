#include "MainUIEditor.h"
#include "RTRStudio.h"

MainUIEditor::MainUIEditor(RTRStudio* appReference)
{
    rtrStudio = appReference;
}

void MainUIEditor::setup() 
{
    rtrStudio->audio.initFMODSystem();

    // GUI
    int offset = 5;

    // Title gui
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
    padStartToggle = guiSound->addToggle("Pad: Start", rtrStudio->jsonSys.getValue("Pad: Start"));
    padEndToggle = guiSound->addToggle("Pad: End", rtrStudio->jsonSys.getValue("Pad: End"));
    fxToggle = guiSound->addToggle("Fx", rtrStudio->jsonSys.getValue("Fx"));
    noiseToggle = guiSound->addToggle("Noise", rtrStudio->jsonSys.getValue("Noise"));
    shepardsToggle = guiSound->addToggle("Shepards", rtrStudio->jsonSys.getValue("Shepards"));

    // general gui
    ofxDatGui* guiGeneral = new ofxDatGui(255 + offset, firstRowHeight);
    initGui(guiGeneral, ":: Adaption ::");
    ofxDatGuiSlider* gainSlider = guiGeneral->addSlider("gain", -90, 0, rtrStudio->jsonSys.getValue("gain")); 
    ofxDatGuiSlider* rangeSlider = guiGeneral->addSlider("length in s", 5, 60, rtrStudio->jsonSys.getValue("length in s"));
    ofxDatGuiSlider* attackSlider = guiGeneral->addSlider("attack", 0.5, 5, rtrStudio->jsonSys.getValue("attack"));         
    ofxDatGuiSlider* releaseSlider = guiGeneral->addSlider("release", 2, 5, rtrStudio->jsonSys.getValue("release"));     

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
    ofxDatGuiSlider* attackDecreaseModifierSlider = guiPositionModulation->addSlider("attack decrease modifier", 0.5, 2, rtrStudio->jsonSys.getValue("attack decrease modifier"));         
    attackDecreaseModifierSlider->setPrecision(2);
    ofxDatGuiSlider* curveSlider = guiPositionModulation->addSlider("curve shape", 0, 1, rtrStudio->jsonSys.getValue("curve shape"));

    // action modulation gui
    ofxDatGui* guiActionModulation = new ofxDatGui(255 + offset, secondRowHeight);
    initGui(guiActionModulation, ":: Action Modulation ::", 1);
    ofxDatGuiSlider* actionModulationThresholdSlider = guiActionModulation->addSlider("am threshold", 0.01, 0.1, rtrStudio->jsonSys.getValue("am threshold"));
    ofxDatGuiSlider* actionModulationLengthSlider = guiActionModulation->addSlider("am length", 0.5, 5, rtrStudio->jsonSys.getValue("am length"));

    // time modulation gui
    ofxDatGui* guiTimeModulation = new ofxDatGui(510 + offset, secondRowHeight);
    initGui(guiTimeModulation, ":: Time Modulation ::", 1);
    ofxDatGuiSlider* timeModulationThresholdSlider = guiTimeModulation->addSlider("tm threshold", 1, 20, rtrStudio->jsonSys.getValue("tm threshold"));
    timeModulationThresholdSlider->setPrecision(1);
    ofxDatGuiSlider* timeModulationLengthSlider = guiTimeModulation->addSlider("tm length", 0.5, 5, rtrStudio->jsonSys.getValue("tm length"));

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
    gainPlotter->setValue(rtrStudio->audio.mainOutputGainAllLayers);
    waveMonitor->setAmplitude(rtrStudio->audio.mainOutputGainAllLayers);
    waveMonitor->setFrequency(rtrStudio->audio.mainFrequencyAllLayers);
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

    rtrStudio->jsonSys.setValue(e.target->getLabel(), e.target->getValue());

    if (label == "gain") 
    {
        rtrStudio->audio.dbToFloat(rtrStudio->jsonSys.getValue(label));
    }
    else if (label == "release") 
    {
        rtrStudio->audio.setReleaseLayers(rtrStudio->jsonSys.getValue(label));
    }
    else if (label == "attack") 
    {
        rtrStudio->audio.setAttack(rtrStudio->jsonSys.getValue(label));
    }
    else if (label == "attack decrease modifier") 
    {
        rtrStudio->audio.setPositionModifier(rtrStudio->jsonSys.getValue(label));
        rtrStudio->audio.calculatePositionGainModulation(rtrStudio->jsonSys.getValue("length in s"));
        rtrStudio->audio.calculatePositionPitchModulation(rtrStudio->jsonSys.getValue("length in s"));
    }
    else if (label == "length in s") 
    {
        rtrStudio->audio.calculatePositionGainModulation(rtrStudio->jsonSys.getValue(label));
        rtrStudio->audio.calculatePositionPitchModulation(rtrStudio->jsonSys.getValue(label));
    }
    else if (label == "offset") 
    {
        rtrStudio->audio.setOffset(rtrStudio->jsonSys.getValue(label));
    }
    else if (label == "curve shape") 
    {
        rtrStudio->audio.setModulationCurveLayers(rtrStudio->jsonSys.getValue(label));
    }
    else if (label == "Position") 
    {
        rtrStudio->audio.setPosition(e.target->getValue());
    }
    else if (label == "tm threshold") 
    {
        rtrStudio->audio.setTimeModulationThreshold(e.target->getValue(), rtrStudio->jsonSys.getValue("length in s"));
    }
    else if (label == "tm length") 
    {
        rtrStudio->audio.setTimeModulationLengthLayers(e.target->getValue(), rtrStudio->jsonSys.getValue("length in s"));
    }
    else if (label == "am threshold") 
    {
        rtrStudio->audio.setActionModulationThreshold(e.target->getValue());
    }
    else if (label == "am length") 
    {
        rtrStudio->audio.setActionModulationLengthLayers(e.target->getValue(), rtrStudio->jsonSys.getValue("length in s"));
    }
    else {
        cout << "Error: slider label not found: " << label << endl;
    }
}

void MainUIEditor::onButtonEvent(ofxDatGuiButtonEvent e)
{
    string label = e.target->getLabel();

    if (label == "Start") 
    {
        rtrStudio->audio.startRiser();
    }
    else if (label == "Impact") 
    {
        rtrStudio->audio.startRelease();
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
    rtrStudio->jsonSys.setValue(e.target->getLabel(), e.target->getChecked());

    rtrStudio->audio.getLayerByName(label)->onOff = e.target->getChecked();
    rtrStudio->audio.debugMessage("onoff for label: " + label + to_string(rtrStudio->audio.getLayerByName(label)->onOff));
}

void MainUIEditor::getProjectLocation()
{
    string file;
    ofFileDialogResult result = ofSystemLoadDialog("Load file");
    if (result.bSuccess) file = result.getPath();
    cout << file << endl;

    rtrStudio->jsonSys.setProjectLocation("Project location", file);
    initialiseAllValues();
}

void MainUIEditor::initialiseAllValues() // TODO: do this automatically
{
    rtrStudio->audio.dbToFloat(rtrStudio->jsonSys.getValue("gain"));
    rtrStudio->audio.setOffset(rtrStudio->jsonSys.getValue("offset"));
    rtrStudio->audio.setModulationCurveLayers(rtrStudio->jsonSys.getValue("curve shape"));
    rtrStudio->audio.setPositionModifier(rtrStudio->jsonSys.getValue("attack decrease modifier"));
    rtrStudio->audio.calculatePositionGainModulation(rtrStudio->jsonSys.getValue("length in s"));
    rtrStudio->audio.calculatePositionPitchModulation(rtrStudio->jsonSys.getValue("length in s"));
    rtrStudio->audio.setAttack(rtrStudio->jsonSys.getValue("attack"));
    rtrStudio->audio.setReleaseLayers(rtrStudio->jsonSys.getValue("release"));
    rtrStudio->audio.setTimeModulationThreshold(rtrStudio->jsonSys.getValue("tm threshold"), rtrStudio->jsonSys.getValue("length in s"));
    rtrStudio->audio.setTimeModulationLengthLayers(rtrStudio->jsonSys.getValue("tm length"), rtrStudio->jsonSys.getValue("length in s"));
    rtrStudio->audio.setActionModulationThreshold(rtrStudio->jsonSys.getValue("am threshold"));
    rtrStudio->audio.setActionModulationLengthLayers(rtrStudio->jsonSys.getValue("am length"), rtrStudio->jsonSys.getValue("length in s"));

    onToggleEvent(padStartToggle); 
    onToggleEvent(padEndToggle);
    onToggleEvent(fxToggle);
    onToggleEvent(noiseToggle);
    onToggleEvent(shepardsToggle);
}