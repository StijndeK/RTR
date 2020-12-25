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
    
    jsonExp.setup();
    mainUIEdit.setup();
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    mainUIEdit.onButtonEvent(e);
}

//--------------------------------------------------------------
void ofApp::update() {
	audio.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	test.draw(0, 0);
    jsonExp.draw();
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
