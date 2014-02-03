#pragma once

#include "tweet3d.h"
#include "ofxVolumetrics.h"
#include "ofxUI.h"
#include "volumeSlice.h"
#include "myCamera.h"
#include "ofUtils.h"



class vizManager {
	
public:
	vizManager();
	~vizManager();

	void setup();
	void update();
	void draw();
	void keyPressed  (int key);	
	
	//UI to remove
	void saveCameraPosition();
	void loadCameraPosition();
	void guiEvent(ofxUIEventArgs &e);
	void setup_GUI1();
	void setup_GUI2();
	ofxUICanvas *gui1;
	ofxUICanvas *gui2;
	
	//UI vars
	float FBOq, Zq, thresh, density, dithering;
	float lastClipPlaneDepth;
	float clipPlaneDepth, azimuth, elevation;
	//Volume Slice
	volumeSlice	sagittal, axial, coronal;
	int sagittalS, axialS, coronalS;
	
	//XML settings
	ofxXmlSettings XML;
	string message;
	
	//Camera
    myCamera cam;
    ofQuaternion camOrientation;
	ofVec3f camPos;
	ofMatrix4x4 posMat;
	bool bcameraMode;
	float rot = .00000001;
	float latitude;
	
	//Volume Rendering
    ofxVolumetrics myVolume;
    unsigned char * volumeData;
    int volWidth, volHeight, volDepth;
    ofxImageSequencePlayer imageSequence;
    bool linearFilter;
	ofFbo myfboRender;

	//Twitter objects
	tweet3d  searchTweetByLocation();
	
private:
	vector<tweet3d> alltweets;
    vector<tweet3d> selectedTweets;
	
	bool bDraw;
};

