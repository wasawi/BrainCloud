#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{	
	// OF vars
		ofSetLogLevel(OF_LOG_VERBOSE);
	//	ofSetLogLevel(OF_LOG_WARNING);
	//	ofSetLogLevel(OF_LOG_ERROR);
	ofEnableSmoothing();
	ofSetFrameRate(30);
	
	ofBackground(100);

//	myGuiManager.setup();
	myVizManager.setup();
//	myTwitterManager.setup("hellocat");
	
}

//--------------------------------------------------------------
void ofApp::update()
{
//	myTwitterManager.update();
//	myGuiManager.update();
//	myVizManager.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{	

	guiManager::getInstance()->draw();
	
	myTwitterManager.draw();
	//myVizManager.draw();
	
}



//--------------------------------------------------------------
void ofApp::exit()
{

	guiManager::getInstance()->exit();
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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
















