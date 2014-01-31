#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{	
	// OF vars
		ofSetLogLevel(OF_LOG_VERBOSE);
	//	ofSetLogLevel(OF_LOG_WARNING);
	//	ofSetLogLevel(OF_LOG_ERROR);
	ofEnableSmoothing();
	ofSetFrameRate(20);
	
	ofBackground(100);

	myGuiManager.setup();
//	myVizManager.setup();
	myTwitterManager.setup("hellocat");
	
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
	myGuiManager.draw();
	myTwitterManager.draw();
	myVizManager.draw();
	
}



//--------------------------------------------------------------
void ofApp::exit()
{
	myGuiManager.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	myVizManager.keyPressed(key);

	/*
    if(gui->hasKeyboardFocus())
    {
        return;
    }
	switch (key)
	{
		case '`':

			break;
			
		case 'f':
			ofToggleFullscreen();
			break;
            
		case 'h':
            gui->toggleVisible();
			break;
            
		case 'p':
			bdrawPadding = !bdrawPadding;
			gui->setDrawWidgetPaddingOutline(bdrawPadding);
			break;
            
		case '[':
			gui->setDrawWidgetPadding(false);
			break;
            
		case ']':
			gui->setDrawWidgetPadding(true);
			break;
            
		case ' ':

			break;

		default:
			break;
	}
	 */
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
















