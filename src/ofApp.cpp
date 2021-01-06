#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() {
	mainUIEdit = new MainUIEditor(this);
}

ofApp::~ofApp() {
	delete mainUIEdit;
}

//--------------------------------------------------------------
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
	mainUIEdit->onButtonEvent(e);
}

//--------------------------------------------------------------
void ofApp::setup() {    
	jsonSys.setup();
	mainUIEdit->setup();
}

//--------------------------------------------------------------
void ofApp::update() {
	audio.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	mainUIEdit->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	audio.playAudioImpacts();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	audio.trigger = 0;
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
