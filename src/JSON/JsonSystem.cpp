#include "JsonSystem.h"

/* TODO:
* have a json file that checks if engine is connected, if true then go to engines json. if not then use local. when switching this bool copy the json over
set a check for if there is an engine connected or there needs to be a standalone
*/

JsonSystem::JsonSystem()
{
}

JsonSystem::~JsonSystem()
{
}

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
    // check if file is available and open
    bool parsingSuccessful = result.open(file);

    if (!parsingSuccessful)
    {
        ofLogError("ofApp::setup") << "Failed to parse JSON" << endl;
    }
}

void JsonSystem::getPath()
{
    ofFileDialogResult result = ofSystemLoadDialog("Load file");
    if (result.bSuccess) {
        file = result.getPath();;
    }

    parseJson();
}

void JsonSystem::setValue(std::string type, float value)
{
    ofLogNotice("set value: " + type + to_string(value));
    result[type] = value;
    result.save(file, true);
}

float JsonSystem::getValue(std::string type)
{
    return result[type].asFloat();
}