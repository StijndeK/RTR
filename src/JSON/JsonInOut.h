#pragma once
#include "ofMain.h"
#include "ofxJSON.h"

class JsonInOut{
public:
    void setup();
    void draw();

    void parseJson();
    void setValue(std::string type, float value);
    void setProjectLocation(std::string type, std::string value);
    float getValue(std::string type);
    std::string getProjectLocation(std::string type);
    
private:
    ofxJSONElement resultLocation;
    ofxJSONElement result;
    ofFile jsonFile;

    std::string locationFile = "projectLocation.json";
    std::string file = "C:\\Users\\sdkst\\Documents\\Unreal Projects\\Project_Rookery\\Plugins\\RTR\\Content\\example.json"; 
};
