#pragma once
#include "ofMain.h"
#include "ofxJSON.h"

class JsonSystem{
public:
    JsonSystem();
    ~JsonSystem();

    void setup();
    void draw();

    void setValue(std::string type, float value); // TODO: struct so that can also be something else then float
    float getValue(std::string type);
    
    ofxJSONElement result;
    ofFile jsonFile;

    std::string file = "C:\\Users\\sdkst\\Documents\\Unreal Projects\\MyProject3\\Plugins\\myPlugin\\Content\\example.json";
};
