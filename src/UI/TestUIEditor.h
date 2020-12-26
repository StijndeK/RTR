#include "ofMain.h"
#include "BaseUIEditor.h"

class TestUIEditor : public BaseUIEditor {
public:
    void setup();
    void onButtonEvent(ofxDatGuiButtonEvent e);
    
    float gain; // TODO: this should be stored in the JSON and read from JSON by audio system (make a convertion function in JSONExport that takes the type and its value)
};
