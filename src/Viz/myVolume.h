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
	CORONAL,	//	diadema
	SAGITTAL,	//	cresta
	AXIAL		//	collar
};

class myVolume : public ofxVolume{
public:
	// slices
	void setup(float bW, float bH);
	void redraw(slice vP, int depth);
	void draw(slice vP);
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
	

//	vector<unsigned char> selectVoxels(vector <ofxPoint> _coord, vector <float> radius);
	
	// selection
	void selectVoxels(vector <ofxIntBox>& boxes);
	void selectVoxels(ofxBox& box);
		
	
	void setSlices(ofVec3f* _planes);
    void setRayPlane(ofPlane* _rayPlane);
    bool getIntersection(ofCamera* cam, ofVec3f& cubeSize, ofVec3f& intersectionPosition);
	
	
protected:
private:
	// methods for slices
	void drawBox();
	bool inside(ofxPoint _coord);
	
	vector <ofxPoint> getVoxelsinRadius(ofxPoint& _coord, float& _radius);



		
	//the box outside the volume useful if volume is non symetrical and you nedd a symetrical box to handle gui or others..

	
	//ofRay objects
    ofVec3f*	planeCoords;
    ofPlane*	rayPlane;
    ofRay       mouseRay;
	

	ofxBox		outerBox;
	
	int			w,h,d;
	int			halfH, halfW, halfD;
	float		boxW, boxH;
	
	// the depth of the current cutting plane
	int coronalS;
	int sagittalS;
	int axialS;

	// one image for each cutting plane
	ofImage coronal;
	ofImage sagittal;
	ofImage axial;
	
	// check if inside
	bool insideCoronal;
	bool insideSagittal;
	bool insideAxial;

	// quick ways of checking if inside radius:
	// http://stackoverflow.com/questions/481144/equation-for-testing-if-a-point-is-inside-a-circle
	
	vector<ofPixels> selectedVoxels;
};















