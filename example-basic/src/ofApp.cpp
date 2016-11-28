#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    link.setup(120, this);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofxAbletonLink::Result res = link.update();
    ofLog(OF_LOG_NOTICE, "Tempo: " + ofToString(link.tempo()) + " Beat: " + ofToString(res.beat) + " Phase: " + ofToString(res.phase));

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::onNumberOfPeersChanged(unsigned long peers){
    ofLog(OF_LOG_NOTICE, "onNumberOfPeersChanged " + ofToString(peers));
}

//--------------------------------------------------------------
void ofApp::onTempoChanged(double tempo){
    ofLog(OF_LOG_NOTICE, "onTempoChanged " + ofToString(tempo));
}
