#include "JSONExport.h"

void JSONExport::setup()
{
    /* TODO: 
    * get the path to the engine version from input field
    * have a json file that checks if engine is connected, if true then go to engines json. if not then use local. when switching this bool copy the json over
    set a check for if there is an engine connected or there needs to be a standalone
    */
    std::string file = "C:\\Users\\sdkst\\Documents\\Unreal Projects\\MyProject3\\Plugins\\myPlugin\\Content\\example.json";

    // check if file is available and open
    bool parsingSuccessful = result.open(file);

    if (parsingSuccessful)
    {
        result["gain"] = 0.8;
        result.save(file, true);
    }
    else
    {
        ofLogError("ofApp::setup")  << "Failed to parse JSON" << endl;
    }
}


void JSONExport::draw()
{
    ofBackground(0);

    ofSetHexColor(0x00FF00);

    std::stringstream ss;

    ss << "gain = " << result["gain"].asDouble() << endl;

    ofDrawBitmapString(ss.str(), 10, 14);
}
