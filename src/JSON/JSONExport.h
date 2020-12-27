#include "ofMain.h"
#include "ofxJSON.h"

class JSONExport{
public:
    void setup();
    void draw();
    
    ofxJSONElement result;
    ofxJSONElement resultEngine;

    ofFile jsonFile;

    void setValue(std::string type, int float); // TODO: struct so that can also be something else then float
};
