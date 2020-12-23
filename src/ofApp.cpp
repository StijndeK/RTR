#include "ofApp.h"

/* TODO:
* - seperate class for audio handling
* - function for initialise, play, stop, set volume
* - try without the plugin in Unreal
*/

//--------------------------------------------------------------
void ofApp::setup() {
	test.load("shoplifters.jpg");
	audio.initFMODSystem();

	ofxDatGui* gui = new ofxDatGui(100, 100);
	gui->addButton("My 1");
	gui->addButton("My 2");
	gui->addHeader(":: Drag Me To Reposition ::");
	gui->addFooter();
	gui->onButtonEvent(this, &ofApp::onButtonEvent);

	//// draw a circle with a radius of 100px
	//// and position it in the middle of the screen //
	//circle = new Circle(100);
	//circle->x = ofGetWidth() / 2;
	//circle->y = ofGetHeight() / 2;

	//// instantiate a gui and a couple of range sliders //
	//gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
	//ofxDatGuiSlider* sx = gui->addSlider("CIRCLE X", 0, ofGetWidth());
	//ofxDatGuiSlider* sy = gui->addSlider("CIRCLE Y", 0, ofGetHeight());

	//// bind the circle's x & y movement to the sliders //
	//sx->bind(circle->x);
	//sy->bind(circle->y);

	myTextFile.open("text.json", ofFile::WriteOnly);
	// myTextFile << "{ "filename1" }";
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
	cout << e.target->getLabel() << endl; // prints "My Button"
}

//--------------------------------------------------------------
void ofApp::update() {
	audio.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	test.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	std::cout << "key pressed" << std::endl;
	audio.playAudio();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
