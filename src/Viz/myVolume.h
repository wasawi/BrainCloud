/*  ofxVolume - draw ofxVolumetric data
 Created by Jordi on 01/04/14.
 */

#pragma once
#include "ofxImageSequencePlayer.h"
#include "ofxBox.h"
#include "ofxVolume.h"
#include "ofxRay.h"


//Volume Cutting Viepoint
enum slice
{
	CORONAL,	//	collar
	SAGITTAL,	//	cresta
	AXIAL		//	diadema
};

class myVolume : public ofxVolume{
public:
	/*
	ofxVolume();
    virtual ~ofxVolume();
	*/
	
	void loadAsRGBA(string path="");
	void loadAsRGBAPow2(string path="");
	void loadAsMono(string path="");
	void setup(float bW, float bH);
	void redraw(slice vP, int depth);
	void destroy();
	
	// Getters
	ofColor getCursorColor();
	ofxIntPoint getCursorCoords(int _index);
	int getCursorID();

	vector <ofxPoint> getVoxelsinBox(ofxIntBox& box);
	
	int getVoxelValue();
	int getVoxelNumber();
	bool getVoxelCoordAndVal(int _index, ofxIntPoint& _coord, int& _val);

	void colourRandomVoxels(int count);
	void colourRandomBoxes(int count);
	
	void draw(slice vP);
//	vector<unsigned char> selectVoxels(vector <ofxPoint> _coord, vector <float> radius);
	
	// selection
	void selectVoxels(vector <ofxIntBox>& boxes);
	void selectVoxels(ofxBox& box);
		
	
	void setSlices(ofVec3f* _planes);
    void setRayPlane(ofPlane* _rayPlane);
    bool getIntersection(ofCamera* cam,ofVec3f &intersectionPosition);
	
	
protected:
private:
	void drawBox();
/*
	void redrawSagittal();
	void redrawAxial();
	void redrawCoronal();
*/
	
	ofxBox outerBox;
	//the box outside the volume useful if volume is non symetrical and you nedd a symetrical box to handle gui or others..

	
	//ofRay objects
    ofVec3f*	planeCoords;
    ofRectangle plane;
    ofPlane*	rayPlane;
    ofRay       mouseRay;
//	ofVec3f volOffset;
//    ofVec3f voxelSize;
    ofVec3f cubeSize;
//    ofVec3f cubePos;
	
	vector <ofxPoint> getVoxelsinRadius(ofxPoint& _coord, float& _radius);
	bool inside(ofxPoint _coord);
	
	int w,h,d;
	int halfH;
	int halfW;
	int halfD;
	float boxW;
	float boxH;
	
	// the depth of the current cutting plane
	int coronalS;
	int sagittalS;
	int axialS;

	// one image for each cutting plane
	ofImage coronal;
	ofImage sagittal;
	ofImage axial;
	
/*	// one pixel array for cutting plane
	ofPixels coronalPixels;
	ofPixels sagittalPixels;
	ofPixels axialPixels;
*/
	
	bool insideCoronal;
	bool insideSagittal;
	bool insideAxial;

	// quick ways of checking if inside radius:
	// http://stackoverflow.com/questions/481144/equation-for-testing-if-a-point-is-inside-a-circle
	
	vector<ofPixels> selectedVoxels;
};















