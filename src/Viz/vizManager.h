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
 
 MNI Volume
 //////////
 
 Defined originally on the MNI 305 Template
 MNI Coordinates (0,0,0) is AC (maybe)
 and simply x,y,z offsets from this.
 
 X (Right+ve,Left–ve)
 Y (Anterior+ve,Posterior–ve)
 Z (Superior+ve,Inferior–ve)
 volume dimensions = 151x188x154
 
 
 Talairach Volume
 ////////////////

 volume dimensions = 141x172x110
 Offset= -70, -102, -42

 how to use Coordinate space
 ///////////////////////////
 we are using Anatomical Plane for volume slices:
 http://en.wikipedia.org/wiki/Anatomical_plane

 coronalDepth	= volCoord.z = moves across volH
 sagittalDepth	= volCoord.x = moves across volW
 axialDepth		= volCoord.y = moves across volD
 
 // 3Dpads coordinates
 front	= x y z
 back	= -x y z
 left	= z y x
 right	= -z y x
 top	= x z y
 bottom = -x z y
 
 */




class vizManager {
	
public:
	vizManager();
	~vizManager();

	void setup();
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
	
	// Volume Rendering
    ofxVolumetrics myVolume;
    unsigned char * volumeData;
    ofxImageSequencePlayer imageSequence;
    bool linearFilter;
	ofFbo myfboRender;
	
	// Volume UI vars
	float FBOq, Zq, thresh, density, dithering;
	float lastClipPlaneDepth;
	float clipPlaneDepth, azimuth, elevation;
	bool bUpdating;
	
	// Volume Slice
	volumeSlice	volume2D;

	// Twitter objects
	tweet3d  searchTweetByLocation();
	vector<tweet3d> alltweets;
    vector<tweet3d> selectedTweets;
	
	// Talairach Atlas
	talairachLabels	talairachAtlas;
	ofxTalairach talClient;
	vector <string> outputLabels;
	
	// updates
	void update();
	void updateCoordinates();
	
	void update2DSlices();
	void updateVolumeSlices();

//	void updatePads();
//	void updateSliders();
	void updateTalCoords();
	void updateTalAtlasLabel();
	void updateTalLabel();
	
	// Vis vars
	bool	bDraw;
	float	initX, initY;
	float	sliderW;
    float	length;
	float	boxW, boxH;
	int		dist;
	
	//All Coordinates
	ofVec3f uiCoord;		// NORMALISED coordinates used in GUI (floats from -1 to 1)
	ofVec3f volCoord;		// coordinates in voxels (integers from 0 to volWidth..)
							// needed to draw the image on position
	ofVec3f volCoordClamp;	// needed to draw the slice
	
	ofVec3f visCoord;		// used in old UI (floats from -boxW/2 to boxW/2)
	
	ofVec3f talOffset;		// the offset of the origin (0,0,0) in Tal coordinates
	ofVec3f talCoord;		// volume coords + offset Tal coords for Tal tables.

	ofVec3f	uiRange;		// maximum and minimum values for UIs
	

	
	int talDrawX;
	int talDrawY;
	
//	ofVec3f volDim;
	int volWidth, volHeight, volDepth;
	int voxelValue;
};






