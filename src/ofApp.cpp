#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{	
	// OF Log Level
		ofSetLogLevel(OF_LOG_VERBOSE);
	//	ofSetLogLevel(OF_LOG_NOTICE);
	//	ofSetLogLevel(OF_LOG_WARNING);
	//	ofSetLogLevel(OF_LOG_ERROR);
	
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
	ofBackground(150);
//	ofBackgroundGradient(150,20);
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
    switch(key)
    {
		case 'f':
			myGUIManager.scrollCanvas->toggleFBO();
			break;
			
		case OF_KEY_F1:
			ofSetLogLevel(OF_LOG_SILENT);
			break;
			
		case OF_KEY_F2:
			ofSetLogLevel(OF_LOG_VERBOSE);
			break;

		case OF_KEY_F3:
			ofSetLogLevel(OF_LOG_NOTICE);
			break;
			
		case OF_KEY_F4:
			ofLog(OF_LOG_VERBOSE, "PubMed");
			ofSetLogLevel("PubMed", OF_LOG_VERBOSE);
			break;

		case OF_KEY_F5:
			ofLog(OF_LOG_VERBOSE, "Twitter");
			ofSetLogLevel("Twitter", OF_LOG_VERBOSE);
			break;
			
		case OF_KEY_F6:
			ofLog(OF_LOG_VERBOSE, "Vis");
			ofSetLogLevel("Vis", OF_LOG_VERBOSE);
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	myTwitterManager.keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
	/*
	if (
		myVizManager.guiSliders->isHit(x, y)	||
		myVizManager.guiVolume->isHit(x, y)		||
		myGUIManager.tabCanvas->isHit(x, y)		||
		myGUIManager.postCanvas->isHit(x, y)	||
		myGUIManager.textInputCanvas->isHit(x, y)||
		myGUIManager.scrollCanvas->isHit(x, y)
		)
	{
		myVizManager.cam.disableMouseInput();
	}
	else
	{
		myVizManager.cam.enableMouseInput();
	}
	 */
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
















