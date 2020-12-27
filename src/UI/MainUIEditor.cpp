#include "MainUIEditor.h"

MainUIEditor::MainUIEditor()
{
}

MainUIEditor::~MainUIEditor()
{
}

void MainUIEditor::setup() {
    audio.initFMODSystem();
    testUI.setup();
    jsonSys.setup();

    // general gui
    ofxDatGui* gui = new ofxDatGui(100, 100);
    gui->onSliderEvent(this, &MainUIEditor::onSliderEvent);
    gui->onButtonEvent(this, &MainUIEditor::onButtonEvent);

    gui->addHeader(":: Drag Me To Reposition ::");
    gui->addFooter();

    // gain slider
    ofxDatGuiSlider* gainSlider = gui->addSlider("gain", 0, 1);
    gainSlider->bind(tGain);
    gainSlider->setValue(jsonSys.getValue("gain")); // init with saved value

    // set button
    gui->addButton("Set");

}

void MainUIEditor::draw() {
    ofBackground(0);
    ofSetHexColor(0x00FF00);

    jsonSys.draw();
}

void MainUIEditor::onSliderEvent(ofxDatGuiSliderEvent e)
{
    cout << e.target->getLabel() << endl; // prints "My Button"
    cout << e.target->getValue() << endl; // prints "My Button"
    jsonSys.setValue(e.target->getLabel(), e.target->getValue());
}

void MainUIEditor::onButtonEvent(ofxDatGuiButtonEvent e)
{
    setAudioValues();
    // testUI.onButtonEvent(e);
    cout << e.target->getLabel() << endl; // prints "My Button"
}

void MainUIEditor::setAudioValues()
{
    audio.setGain(jsonSys.getValue("gain"));
}

void MainUIEditor::keyPressed(int key) {
    audio.playAudio();
}

