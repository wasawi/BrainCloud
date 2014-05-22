#pragma once

#include "ofMain.h"
#include "tweet3d.h"
#include "ofxVolumetrics.h"
#include "ofxUI.h"
//#include "volume.h"
#include "myVolume.h"
#include "ofUtils.h"
#include "talairachLabels.h"
#include "ofxTalairach.h"
#include "ofxRay.h"
#include "ofxCameraSaveLoad.h"
#include "selectionManager.h"

//#include "localizationManager.h"

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
	void mousePressed(ofMouseEventArgs& e);
	void keyPressed  (int key);

	void select();
	void moveCursor();
	void addSelection();
	void selectVoxels();
	
	//Camera
	ofEasyCam cam;
	
	// guis
	ofxUICanvas *guiVolume;
	ofxUICanvas *guiSliders;
	
	//states
	bool	bMovingCursor;
	bool	bSelecting;
	bool	bActive;
	bool	bDraw;
	
private:

	// inits
	void initGui();
	void initVolume();
	void initVolumeRendering();
	void initRayPlane();
	void initTalairach();
	
	// GUI
	void guiEvent(ofxUIEventArgs &e);
	void setup_guiVolume();
	void setup_guiSliders();

	// updates
	void update();
	void updateCoordinates();
	void updateSlicesImage();
	void updateSlices2Volume();
	void updateVolume2Slices();
	
	
	void updateTalCoords();
	void updateTalAtlasLabel();
	void updateTalLabel();
	
	void voxelToVector(ofVec3f& voxel);

	//	draws
	void drawVolume();
	void drawSlices();
	void drawTalairach();
	void drawMesh();
	void drawNearestPoint();
	void drawSelection();
	
	// GUI vars
	float	initX, initY;
	float	sliderW;
    float	length;
	float	boxW, boxH;
	int		dist;
	int		talDrawX;
	int		talDrawY;
	bool	allowEvent;
	
	//XML settings
	ofxXmlSettings XML;
	string message;
	
	// Volume Rendering
    ofxVolumetrics volRender;
	ofFbo myfboRender;
	ofVec3f cubeSize;

	// Volume Slice
	myVolume	vol;
	int volWidth, volHeight, volDepth;
	ofVec3f volSize;
	
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
	
	
	//All Coordinates
	ofVec3f	uiRange;	// maximum and minimum values for UIs
	ofVec3f uiCoord;	// NORMALISED coordinates used in GUI (floats from -1 to 1)
	ofVec3f volCoord;	// coordinates in voxels (integers from 0 to volWidth..)
						// needed to draw the image on position
	ofVec3f uiClamp;	// same as ui but using values only inside volume

	ofVec3f talOffset;	// the offset of the origin (0,0,0) in Tal coordinates
	ofVec3f talCoord;	// volume coords + offset Tal coords for Tal tables.

	// ofxRay objects
	ofPlane rayPlane;
	ofVec3f intersectionPosition;
	
	// Mesh
	void createPointCloud();
	ofMesh mesh;
	ofMesh meshWhite;

	//	localizationManager words;
	
	// selection
	selectionManager selection;
};






