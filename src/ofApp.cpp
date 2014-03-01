#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{	
	// OF vars
	//	ofSetLogLevel(OF_LOG_VERBOSE);
		ofSetLogLevel(OF_LOG_NOTICE);
	//	ofSetLogLevel(OF_LOG_WARNING);
	//	ofSetLogLevel(OF_LOG_ERROR);
	ofEnableSmoothing();
	ofSetFrameRate(30);
	
	string const CONSUMER_KEY = "zSrKv91OmRK1F2wgqXpvQ";
	string const CONSUMER_SECRET = "vUMkjJE70B4xC4nWpMxtScgZYjqzJsceGUbyE3iQ";
	myTwitterManager.setup(CONSUMER_KEY, CONSUMER_SECRET);
//	guiManager.setup();
	myVizManager.setup();
}

//--------------------------------------------------------------
void ofApp::update()
{
	myTwitterManager.update();
//	myGuiManager.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
//	ofBackground(50);
	ofBackgroundGradient(150,20);
	myVizManager.draw();
	myGUIManager.draw();
	myTwitterManager.draw();

}

//--------------------------------------------------------------
void ofApp::exit()
{

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	myVizManager.keyPressed(key);

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	myTwitterManager.keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
	ofSetWindowShape( WINDOW_WIDTH, WINDOW_HEIGHT);
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
















