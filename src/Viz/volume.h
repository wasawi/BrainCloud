/*  volume - draw volumetric data on a 2d plane
 */

#pragma once
//#include "ofxTexture3d.h"
#include "ofxImageSequencePlayer.h"

//Volume Cutting Viepoint
enum viewPoint
{
	CORONAL,	//	collar
	SAGITTAL,	//	cresta
	AXIAL		//	diadema
};

class volume{
public:
	
	volume();
    virtual ~volume();

	void load(string path="");
	void setup(float bW, float bH);
	void redraw(viewPoint vP, int depth);

	
	// Getters
	unsigned char* getVoxels();
	ofVec3f getVolSize();
	ofVec3f getVolPos();
	
	int getVoxelValue();
	int getVoxelNumber();
	ofVec3f getVoxelCoordinates(int _index);
	bool getVoxelCoordAndVal(int _index, ofVec3f& _coord, int& _val);
	
	void draw(viewPoint vP);
	
protected:
private:
	void drawBox();
	void redrawSagittal();
	void redrawAxial();
	void redrawCoronal();

    ofxImageSequencePlayer imageSequence;

	unsigned char * voxels;
	ofPixels myPixels;

	int volWidth, volHeight, volDepth;
	ofVec3f	volSize;
	ofVec3f	volPos;

	int renderWidth, renderHeight;
	
	/*
	int volWidth, volHeight, volDepth;
	int voxelValue;
	int voxelNumber;
	*/
	
	int halfH;
	int halfW;
	int halfD;
	
	float boxW;
	float boxH;

	// the depth of the current slice
	int coronalS;
	int sagittalS;
	int axialS;

	// one image for eaxh point of view
	ofImage coronal;
	ofImage sagittal;
	ofImage axial;
	
	// one pixel array for each image
	ofPixels coronalPixels;
	ofPixels sagittalPixels;
	ofPixels axialPixels;
	
	ofColor lineColor;
	
	bool insideCoronal;
	bool insideSagittal;
	bool insideAxial;
	
	
	
	
	
	//	void drawSagittal(float x, float y, float z);
	//	void drawAxial(float x, float y, float z);
	//	void drawCoronal(float x, float y, float z);
};