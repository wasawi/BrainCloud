/*  VolumeSlice - draw volumetric data on a 2d plane
 */

#pragma once
#include "ofxTexture3d.h"

//Volume Cutting Viepoint
enum viewPoint
{
	SAGITTAL,	//	cresta
	AXIAL,		//	diadema
	CORONAL		//	collar
};

class volumeSlice : public  ofxTexture3d, ofTexture{
public:
	
	volumeSlice();
    virtual ~volumeSlice();
	void setup(unsigned char * data, int w, int h, int d, enum viewPoint);
	void draw(float x, float y, int zTexOffset);
	void drawSagital(float x, float y, int zTexOffset);
	void drawAxial(float x, float y, int zTexOffset);
	void drawCoronal(float x, float y, int zTexOffset);
	
protected:
private:
	
	unsigned char * myData;
	ofImage myImage;
	ofPixels myPixels;
	viewPoint viewPoint;
	int volWidth, volHeight, volDepth;
	int renderWidth, renderHeight;
	
};