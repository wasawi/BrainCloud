#define USE_FTGL

#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"


//========================================================================
int main( ){

    ofAppGlutWindow window;
//	window.setGlutDisplayString("rgba double samples>=6 depth");        
	ofSetupOpenGL(&window, 1620, 800, OF_WINDOW);			// <-------- setup the GL context
	ofRunApp( new ofApp());
}
