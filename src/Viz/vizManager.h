#pragma once

#include "tweet3d.h"
#include "ofxVolumetrics.h"
#include "ofxUI.h"
#include "volumeSlice.h"
#include "myCamera.h"
#include "ofUtils.h"
#include "talairachLabels.h"
#include "ofxTalairach.h"


/*
 
 ************* about MNI
 Defined originally on the MNI 305 Template
 MNI Coordinates (0,0,0) is AC (maybe)
 and simply x,y,z offsets from this.
 
 X (Right+ve,Left–ve)
 Y (Anterior+ve,Posterior–ve)
 Z (Superior+ve,Inferior–ve)
 
 volume dimensions = 151x188x154
 
 ************** about Talairach
 volume dimensions = 141x172x110
 Offset= -70, -102, -42
 
 */


class vizManager {
	
public:
	vizManager();
	~vizManager();

	void setup();
	void update();
	void draw();
	void initVolume();
	void keyPressed  (int key);

private:

	// GUI
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
	
	//XML settings
	ofxXmlSettings XML;
	string message;
	
	//Camera
//    ofEasyCam cam;
	myCamera cam;
    ofQuaternion camOrientation;
	ofVec3f camPos;
	ofMatrix4x4 posMat;
	bool bcameraMode;
	float latitude;
	
	//Volume Rendering
    ofxVolumetrics myVolume;
    unsigned char * volumeData;
    ofxImageSequencePlayer imageSequence;
    bool linearFilter;
	ofFbo myfboRender;

	//Twitter objects
	tweet3d  searchTweetByLocation();
	vector<tweet3d> alltweets;
    vector<tweet3d> selectedTweets;
	
	//Talairach Atlas
	talairachLabels	talairachAtlas;
	ofxTalairach talClient;
	vector <string> outputLabels;
	
	//updates
	void updateCoordinates();
	void updateSlices();
	void updatePads();
	void updateSliders();
	void updateTalCoords();
	void updateTalAtlasLabel();
	void updateTalLabel();
	void updateVolumeSlices();
	
	// Vis vars
	bool	bDraw;
	float	initX, initY;
	float	sliderW;
    float	length;
	float	boxW, boxH;
	int		dist;
	
	//All Coordinates
	ofVec3f volCoord;		// coordinates in Volume (integers from 0 to volWidth..)
	ofVec3f visCoord;		// coordinates used in GUI (floats from -1 to 1)
	ofVec3f volCoordClamp;	// the clamped coordinates of volCoord
	ofVec3f talOffset;		// the offset of the origin (0,0,0) in Tal coordinates
	ofVec3f talCoord;		// volume coords + offset Tal coords for Tal tables.
	
	/* how to use Coordinate space
	 we are using Anatomical Plane for volume slices:
	 http://en.wikipedia.org/wiki/Anatomical_plane
	 
	 we are using
	 
	 X	=
	 Y	=
	 Z	=
	
	*/
	
	
	int TalDrawX;
	int TalDrawy;
	
//	ofVec3f volDim;
	int volWidth, volHeight, volDepth;
	int voxelValue;
};






