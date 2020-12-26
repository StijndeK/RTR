#include "MainUIEditor.h"

void MainUIEditor::setup() {
    testUI.setup();
}

void MainUIEditor::onButtonEvent(ofxDatGuiButtonEvent e)
{
    testUI.onButtonEvent(e);
    // cout << e.target->getLabel() << endl; // prints "My Button"
}

