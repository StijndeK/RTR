#include "ofMain.h"
#include "ofxJSON.h"

class JsonSystem{
public:
    void setup();
    void draw();
    
    ofxJSONElement result;
    ofxJSONElement resultEngine;

    ofFile jsonFile;

    void setValue(std::string type, float value); // TODO: struct so that can also be something else then float
    float getValue(std::string type);

    std::string file = "C:\\Users\\sdkst\\Documents\\Unreal Projects\\MyProject3\\Plugins\\myPlugin\\Content\\example.json";
};
