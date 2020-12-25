#include "MainUIEditor.h"

void MainUIEditor::setup() {
    ofxDatGui* gui = new ofxDatGui(100, 100);
    gui->addButton("My 1");
    gui->addButton("My 2");
    gui->addHeader(":: Drag Me To Reposition ::");
    gui->addFooter();
    gui->onButtonEvent(this, &MainUIEditor::onButtonEvent);

    //// draw a circle with a radius of 100px
    //// and position it in the middle of the screen //
    //circle = new Circle(100);
    //circle->x = ofGetWidth() / 2;
    //circle->y = ofGetHeight() / 2;

    //// instantiate a gui and a couple of range sliders //
    //gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
    //ofxDatGuiSlider* sx = gui->addSlider("CIRCLE X", 0, ofGetWidth());
    //ofxDatGuiSlider* sy = gui->addSlider("CIRCLE Y", 0, ofGetHeight());

    //// bind the circle's x & y movement to the sliders //
    //sx->bind(circle->x);
    //sy->bind(circle->y);
    
}

void MainUIEditor::onButtonEvent(ofxDatGuiButtonEvent e)
{
    cout << e.target->getLabel() << endl; // prints "My Button"
}

