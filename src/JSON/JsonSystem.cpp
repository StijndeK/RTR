#include "JsonSystem.h"

/* TODO:
* have a json file that checks if engine is connected, if true then go to engines json. if not then use local. when switching this bool copy the json over
set a check for if there is an engine connected or there needs to be a standalone
*/

void JsonSystem::setup()
{
    parseJson();
}

void JsonSystem::draw()
{
    // draw output
    std::stringstream ss;
    ss << "gain = " << result["gain"].asDouble() << endl;
    ofDrawBitmapString(ss.str(), 10, 14);
}

void JsonSystem::parseJson()
{
    // load project location
    bool parsingSuccessful1 = resultLocation.open(locationFile);

    if (!parsingSuccessful1)
    {
        ofLogError("ofApp::setup") << "Failed to parse JSON" << endl;
    }

    // set the new location
    file = resultLocation["Project location"].asString();

    // check if file is available and open
    bool parsingSuccessful = result.open(file);

    if (!parsingSuccessful)
    {
        ofLogError("ofApp::setup") << "Failed to parse JSON" << endl;
    }
}

void JsonSystem::setValue(std::string type, float value)
{
    ofLogNotice("set value: " + type + to_string(value));
    result[type] = value;
    result.save(file, true);
}

void JsonSystem::setProjectLocation(std::string type, std::string value)
{
    ofLogNotice("set value: " + type + value);
    resultLocation[type] = value;
    resultLocation.save(locationFile, true);

    parseJson();
}

float JsonSystem::getValue(std::string type)
{
    return result[type].asFloat();
}

std::string JsonSystem::getProjectLocation(std::string type)
{
    return resultLocation[type].asString();
}