#pragma once
#include "ofMain.h"


// Box
void drawBox(ofVec3f position, float size);
void drawBoxAxis(ofVec3f position, float size);

// Sphere
void drawSphere(ofVec3f position, float radius);
void drawSphereAxis(ofVec3f position, float radius);

// Volume
void drawSlices_( ofVec3f planeCoords, float size);
void drawPlanes( ofVec3f planeCoords, ofVec3f size);

	
// OF
void ofScale(float Amnt);
void ofScale(ofVec3f Amnt);

void ofDrawBox(const ofPoint& position, const ofPoint& size);
