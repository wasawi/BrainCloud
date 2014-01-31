#pragma once

#include "tweet3d.h"
//#include "ofxVolumetrics.h"
#include "ofxUI.h"

class vizManager {
	
public:


	void setup();
	void update();
	void draw();

	void keyPressed  (int key);	
	
	//UI to remove
	void saveCameraPosition();
	void loadCameraPosition();
	// void dragEvent(ofDragInfo dragInfo);
 	// void gotMessage(ofMessage msg);
	void guiEvent(ofxUIEventArgs &e);
	void setup_GUI1();
	ofxUICanvas *gui1;
	ofColor backgroundColor;
	
	//XML settings
	ofxXmlSettings XML;
	string message;
	
	//Camera
    ofEasyCam cam;
    ofQuaternion camOrientation;
	ofVec3f camPos;
	ofMatrix4x4 posMat;
	bool bcameraMode;
	
	//Volume Rendering
    ofxVolumetrics myVolume;
    unsigned char * volumeData;
    int volWidth, volHeight, volDepth;
    ofxImageSequencePlayer imageSequence;
    bool linearFilter;
	float lastClipPlaneDepth;
	float clipPlaneDepth, azimuth, elevation;
	ofFbo myfboRender;

	//Vars from UI to A-me
	float FBOq, Zq, thresh, density, dithering;

	//Twitter objects
	tweet3d  searchTweetByLocation();
	
	
private:
	vector<tweet3d> alltweets;
    vector<tweet3d> selectedTweets;
};