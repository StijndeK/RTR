#include "ofMain.h"
#include "BaseUIEditor.h"

class MainUIEditor : public BaseUIEditor {
public:
    void setup();
    void onButtonEvent(ofxDatGuiButtonEvent e);
};

