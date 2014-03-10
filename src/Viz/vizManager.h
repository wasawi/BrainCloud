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
 
 X (Right+ve,Left�ve)
 Y (Anterior+ve,Posterior�ve)
 Z (Superior+ve,Inferior�ve)
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
			
	// GUI vars
	bool	bDraw;
	float	initX, initY;
	float	sliderW;
    float	length;
	float	boxW, boxH;
	int		dist;
	int		talDrawX;
	int		talDrawY;
	
	//XML settings
	ofxXmlSettings XML;
	string message;
	
	//Camera
//	ofEasyCam cam;
	myCamera cam;
	ofMatrix4x4 posMat;
	bool bcameraMode;
	float latitude;


	// Volume Rendering
    ofxVolumetrics myVolume;
    unsigned char * volumeData;
    ofxImageSequencePlayer imageSequence;
	ofFbo myfboRender;

	// Volume Slice
	volumeSlice	volume2D;
	int volWidth, volHeight, volDepth;
	int voxelValue;
	
	// Volume rendering UI vars
	float FBOq, Zq, thresh, density, dithering;
	float lastClipPlaneDepth;
	float clipPlaneDepth, azimuth, elevation;
    bool linearFilter;

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
	void update2DVolume();
	void updateVolumeCoords();
	
	void updateTalCoords();
	void updateTalAtlasLabel();
	void updateTalLabel();
	
	//All Coordinates
	ofVec3f	uiRange;	// maximum and minimum values for UIs
	ofVec3f uiCoord;	// NORMALISED coordinates used in GUI (floats from -1 to 1)
	ofVec3f volCoord;	// coordinates in voxels (integers from 0 to volWidth..)
						// needed to draw the image on position
	ofVec3f uiClamp;	// same as ui but using values only inside volume

	ofVec3f talOffset;	// the offset of the origin (0,0,0) in Tal coordinates
	ofVec3f talCoord;	// volume coords + offset Tal coords for Tal tables.
};






