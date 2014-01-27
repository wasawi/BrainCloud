#ifndef TESTAPP
#define TESTAPP

#include "ofMain.h"

#include "ofxExampleTwitterClient.h"
#include "ofxJSONElement.h"

#include "tweetManager.h"
#include "guiManager.h"

class ofApp : public ofBaseApp
{
	public:
	void setup();
	void update();
	void draw();
	void exit(); 

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	tweetManager mytwitterManager;
	//guiTwitterManager // a candidate name for a new class manger
	guiManager myguimanager;


	
	bool drawFill;
	float red, green, blue, alpha; 	
        
    ofColor backgroundColor; 
    float radius; 
    int resolution;
    ofPoint position;
    

    	
//	ofxOAuth
	ofxExampleTwitterClient client;
	
//	ofxJSON
	ofxJSONElement jsonParser;
	bool bParsed;

//	Twitter
	void setupTwitter();
	void postTweet();

};

#endif