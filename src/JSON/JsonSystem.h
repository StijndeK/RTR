#pragma once
#include "ofMain.h"
#include "ofxJSON.h"

class JsonSystem{
public:
    void setup();
    void draw();

    void parseJson();
    void getPath();
    void setValue(std::string type, float value); // TODO: struct so that value can be multiple types of values
    float getValue(std::string type);
    
private:
    ofxJSONElement result;
    ofFile jsonFile;

    std::string file = "C:\\Users\\sdkst\\Documents\\Unreal Projects\\Project_Rookery\\Plugins\\RTR\\Content\\example.json"; // TODO: get from input
};
