#pragma once
#include "ofMain.h"
#include "ofxJSON.h"

class JsonSystem{
public:
    JsonSystem();
    ~JsonSystem();

    void setup();
    void draw();

    void parseJson();
    void getPath();
    void setValue(std::string type, float value); // TODO: struct so that value can be multiple types of values
    float getValue(std::string type);
    
private:
    ofxJSONElement result;
    ofFile jsonFile;

    std::string file = "C:\\Users\\sdkst\\Documents\\Unreal Projects\\MyProject3\\Plugins\\myPlugin\\Content\\example.json"; // TODO: get from input
};
