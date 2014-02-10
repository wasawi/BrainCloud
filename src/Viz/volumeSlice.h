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
	void setup(unsigned char * data, int w, int h, int d, float bW, float bH);
	void draw(int zTexOffset, viewPoint vP);
	void drawSagittal(float x, float y, int zTexOffset);
	void drawAxial(float x, float y, int zTexOffset);
	void drawCoronal(float x, float y, int zTexOffset);
protected:
private:
	
	unsigned char * myData;
	ofImage myImage;
	ofPixels myPixels;
	viewPoint myViewPoint;
	int volWidth, volHeight, volDepth;
	int renderWidth, renderHeight;
	
	int halfH;
	int halfW;
	int halfD;
	
	float boxW;
	float boxH;

	int axialS;
	int sagittalS;
	int coronalS;
	
	ofColor lineColor;

};