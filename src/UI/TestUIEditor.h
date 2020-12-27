#include "ofMain.h"
#include "BaseUIEditor.h"

class TestUIEditor : public BaseUIEditor {
public:
    void setup();
    void onButtonEvent(ofxDatGuiButtonEvent e);
};
