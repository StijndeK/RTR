#include "TestUIEditor.h"

void TestUIEditor::setup() {
    ofxDatGui* gui = new ofxDatGui(100, 100);
//    gui->addButton("My 1");
//    gui->addButton("My 2");
//    gui->addHeader(":: Drag Me To Reposition ::");
//    gui->addFooter();
//    gui->onButtonEvent(this, &MainUIEditor::onButtonEvent);

    //// draw a circle with a radius of 100px
    //// and position it in the middle of the screen //
    circle = new Circle(100);
    circle->x = ofGetWidth() / 2;
    circle->y = ofGetHeight() / 2;

    // instantiate a gui and a couple of range sliders //
    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
    ofxDatGuiSlider* gainSlider = gui->addSlider("CIRCLE X", 0, ofGetWidth());
    //ofxDatGuiSlider* sy = gui->addSlider("CIRCLE Y", 0, ofGetHeight());
    
    gainSlider->bind(gain);

    // bind the circle's x & y movement to the sliders //
//    sx->bind(circle->x);
    sy->bind(circle->y);
}

void TestUIEditor::onButtonEvent(ofxDatGuiButtonEvent e)
{
    // cout << e.target->getLabel() << endl; // prints "My Button"
}

