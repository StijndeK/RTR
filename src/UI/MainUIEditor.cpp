#include "MainUIEditor.h"

MainUIEditor::MainUIEditor()
{
}

MainUIEditor::~MainUIEditor()
{
}

void MainUIEditor::setup() {
    testUI.setup();
    jsonSys.setup();

    // general gui
    ofxDatGui* gui = new ofxDatGui(100, 100);
    gui->addHeader(":: Drag Me To Reposition ::");
    gui->addFooter();

    // gain slider
    ofxDatGuiSlider* gainSlider = gui->addSlider("gain", 0, 1);
    gainSlider->bind(tGain);
    gainSlider->setValue(jsonSys.getValue("gain")); // init with saved value

    gui->onSliderEvent(this, &MainUIEditor::onSliderEvent);
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
    testUI.onButtonEvent(e);
    // cout << e.target->getLabel() << endl; // prints "My Button"
}

