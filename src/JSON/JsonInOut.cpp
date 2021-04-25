#include "JsonInOut.h"

// TODO: have a json file that checks if engine is connected, if true then go to engines json. if not then use local. when switching this bool copy the json over, set a check for if there is an engine connected or there needs to be a standalone

void JsonInOut::setup()
{
    parseJson();
}

void JsonInOut::draw()
{
    std::stringstream ss;
    ss << "gain = " << result["gain"].asDouble() << endl;
    ofDrawBitmapString(ss.str(), 10, 14);
}

void JsonInOut::parseJson()
{
    // Load project location.
    if (!resultLocation.open(locationFile)) ofLogError("ofApp::setup") << "Failed to parse JSON" << endl;

    // Save the location.
    file = resultLocation["Project location"].asString();

    // Check if file is available and open.
    if (!result.open(file)) ofLogError("ofApp::setup") << "Failed to parse JSON" << endl;
}

void JsonInOut::setValue(std::string type, float value)
{
    ofLogNotice("set value: " + type + to_string(value));
    result[type] = value;
    result.save(file, true);
}

void JsonInOut::setProjectLocation(std::string type, std::string value)
{
    ofLogNotice("set value: " + type + value);
    resultLocation[type] = value;
    resultLocation.save(locationFile, true);

    parseJson();
}

float JsonInOut::getValue(std::string type)
{
    return result[type].asFloat();
}

std::string JsonInOut::getProjectLocation(std::string type)
{
    return resultLocation[type].asString();
}