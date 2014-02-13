#pragma once

#include "tweet3d.h"
#include "ofxVolumetrics.h"
#include "ofxUI.h"
#include "volumeSlice.h"
#include "myCamera.h"
#include "ofUtils.h"
#include "talairachLabels.h"


class vizManager {
	
public:
	vizManager();
	~vizManager();

	void setup();
	void update();
	void draw();
	void initVolume();
	void keyPressed  (int key);
	
	//
	void updateLabel();
	void saveCameraPosition();
	void loadCameraPosition();
	void guiEvent(ofxUIEventArgs &e);
	void setup_guis();
	void setup_guiVolume();
	void setup_guiSliders();
	ofxUICanvas *guiVolume;
	ofxUICanvas *guiSliders;
	
	//UI vars
	float FBOq, Zq, thresh, density, dithering;
	float lastClipPlaneDepth;
	float clipPlaneDepth, azimuth, elevation;
	
	//Volume Slice
	volumeSlice	volume2D;
	
	//All Coordinates
	int volWidth, volHeight, volDepth;		//volume dimensions in indices
	int isliceX, isliceY, isliceZ;			//current slices in vol indices
	float fsliceX, fsliceY, fsliceZ;		//current slices in floats (norm)
	//	int coronal, sagittal, axial;		//int in 2dVolume
	float coronalS, sagittalS, axialS;		//floats in slider
	
	/*
	 Defined originally on the MNI 305 Template
	 MNI Coordinates (0,0,0) is AC (maybe) 
	 and simply x,y,z offsets from this.
	 
	 X (Right+ve,Left–ve)
	 Y (Anterior+ve,Posterior–ve) 
	 Z (Superior+ve,Inferior–ve)
	*/
	
	
	//XML settings
	ofxXmlSettings XML;
	string message;
	
	//Camera
    myCamera cam;
    ofQuaternion camOrientation;
	ofVec3f camPos;
	ofMatrix4x4 posMat;
	bool bcameraMode;
	float rot;
	float latitude;
	
	//Volume Rendering
    ofxVolumetrics myVolume;
    unsigned char * volumeData;
    ofxImageSequencePlayer imageSequence;
    bool linearFilter;
	ofFbo myfboRender;

	//Twitter objects
	tweet3d  searchTweetByLocation();
	
	//Talairach Atlas
	talairachLabels	talairachAtlas;
	
private:
	vector<tweet3d> alltweets;
    vector<tweet3d> selectedTweets;
	void updateCoordinates();
	void updatePads();
	void updateSliders();
	
	bool	bDraw;
	float	initX, initY;
	float	sliderW;
    float	length;
	float	boxW;
	float	boxH;
	int		dist;
//	int		slider;
	
	int pixelValue;
};

