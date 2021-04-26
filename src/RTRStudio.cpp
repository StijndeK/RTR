#include "RTRStudio.h"


RTRStudio::RTRStudio() 
{
	mainUIEdit = new MainUIEditor(this);
}

RTRStudio::~RTRStudio() 
{
	delete mainUIEdit;
}

void RTRStudio::onButtonEvent(ofxDatGuiButtonEvent e)
{
	mainUIEdit->onButtonEvent(e);
}

void RTRStudio::setup() 
{    
	jsonSys.setup();
	mainUIEdit->setup();
}

void RTRStudio::update() 
{
	audio.update();
	mainUIEdit->update();
}

void RTRStudio::draw() 
{
	mainUIEdit->draw();
}

void RTRStudio::keyPressed(int key) 
{
	audio.soundDesign();
}
