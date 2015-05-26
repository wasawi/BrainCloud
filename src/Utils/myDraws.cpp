#include "myDraws.h"


//--------------------------------------------------------------
void drawBoxAxis(ofVec3f position, float size){
		
	ofPushMatrix();
	ofTranslate(position);
	
	ofPushStyle();
	ofNoFill();
	
	// daw cirles
	ofPushView();
	
	// draw x axis
	ofSetColor(ofColor::red);
	ofDrawBox(ofVec3f(0), size, size,0);
	// draw y axis
	ofRotateY(90);
	ofSetColor(ofColor::green);
	ofDrawBox(ofVec3f(0), size, size,0);
	// draw z axis
	ofRotateX(90);
	ofSetColor(ofColor::blue);
	ofDrawBox(ofVec3f(0), size, size,0);
	
	ofPopView();
	
	// draw x axis
	ofSetColor(ofColor::red);
	ofLine(0, 0, 0, size/2, 0, 0);
	// draw y axis
	ofSetColor(ofColor::green);
	ofLine(0, 0, 0, 0, size/2, 0);
	// draw z axis
	ofSetColor(ofColor::blue);
	ofLine(0, 0, 0, 0, 0, size/2);
	
	ofPopStyle();
	ofPopMatrix();
}

//--------------------------------------------------------------
void drawSphereAxis(ofVec3f position, float radius){
		
	ofPushMatrix();
	ofTranslate(position);
	
	ofPushStyle();
	ofNoFill();
	
	// daw cirles
	ofPushView();
	
	// draw x axis
	ofSetColor(ofColor::red);
	ofCircle(0, 0, 0, radius);
	// draw y axis
	ofRotateY(90);
	ofSetColor(ofColor::green);
	ofCircle(0, 0, 0, radius);
	// draw z axis
	ofRotateX(90);
	ofSetColor(ofColor::blue);
	ofCircle(0, 0, 0, radius);
	
	ofPopView();
	
	// draw x axis
	ofSetColor(ofColor::red);
	ofLine(0, 0, 0, radius, 0, 0);
	// draw y axis
	ofSetColor(ofColor::green);
	ofLine(0, 0, 0, 0, radius, 0);
	// draw z axis
	ofSetColor(ofColor::blue);
	ofLine(0, 0, 0, 0, 0, radius);
	
	ofPopStyle();
	ofPopMatrix();
}


//--------------------------------------------------------------
void drawBox(ofVec3f position, float size){
	
//	ofPushMatrix();
//	ofTranslate(position);
	
	ofPushStyle();
	ofSetColor(255,50);
	ofDrawBox(position, size);
	
	ofSetColor(255);
	ofNoFill();
	ofDrawBox(position, size);
	ofPopStyle();
	
	
	
//	ofPopMatrix();

}

//--------------------------------------------------------------
void drawSphere(ofVec3f position, float radius){
	
	ofPushStyle();
	ofSetColor(255,50);
	ofCircle(position, radius);
	
	ofSetColor(255);
	ofNoFill();
	ofCircle(position, radius);
	ofPopStyle();

}

//--------------------------------------------------------------
void drawSlices_(ofVec3f planeCoords, float size)
{
	float f = 1.011;
	
	ofVec3f coronalPlane	 = ofVec3f(0,0,planeCoords.z);
	ofVec3f sagittalPlane	 = ofVec3f(0,0,planeCoords.x);
	ofVec3f axialPlane		 = ofVec3f(0,0,planeCoords.y);
	
	/*
	coronalPlane		 *= sizeFactor;
	sagittalPlane		 *= sizeFactor;
	axialPlane			 *= sizeFactor;
	*/

	ofPushStyle();
	ofSetColor(150,0,0);
	ofNoFill();
	ofSetPlaneResolution(2, 2);
	ofPushMatrix();
//	ofScale(cubeSize.x*f,cubeSize.y*f,cubeSize.z*f);
	ofScale(size*f);
	
	// Draw Slices
	ofPushMatrix();
	ofRotateY(90);
	ofDrawPlane(sagittalPlane, size, size);
	ofPopMatrix();
	
	ofScale(-1.,-1.,-1.);
	ofDrawPlane(axialPlane, size, size);
	
	ofRotateX(90);
	ofDrawPlane(coronalPlane, size, size);
	
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void drawPlanes(ofVec3f planeCoords, ofVec3f size)
{
	
	float f = 1.011;
	
//	ofVec3f pos = *planeCoords*sizeFactor;
//	ofVec3f s = ofVec3f(size, size, 0);
	
	ofPushStyle();
	ofSetLineWidth(1);
	ofSetColor(255,255,0);
	ofNoFill();
	
	ofPushMatrix();
//	ofScale(cubeSize.x*f,cubeSize.y*f,cubeSize.z*f);
//	ofVec3f coronalPlane	 = ofVec3f(0,0,planeCoords.z);
//	ofVec3f sagittalPlane	 = ofVec3f(0,0,planeCoords.x);
//	ofVec3f axialPlane		 = ofVec3f(0,0,planeCoords.y);
	
	/*
	coronalPlane		 *= sizeFactor;
	sagittalPlane		 *= sizeFactor;
	axialPlane			 *= sizeFactor;
	*/
	
//	ofPushMatrix();
//	ofRotateY(90);
	ofScale(f*size);
	
	ofDrawBox( 0, 0, planeCoords.x, 0, size.y, size.z);
//	ofDrawBox(sagittalPlane, size);
//	ofPopMatrix();
	
//	ofScale(-1.,-1.,-1.);
	ofDrawBox( 0, 0, planeCoords.y, size.x, size.y, 0);
//	ofDrawBox(axialPlane, size);
	
//	ofRotateX(90);
	ofDrawBox( 0, 0, planeCoords.z, size.x, 0, size.z);
//	ofDrawBox(coronalPlane, size);
	
	ofPopMatrix();
	ofPopStyle();
}


//----------------------------------------------------------
void ofScale(float Amnt){
	ofScale(Amnt, Amnt, Amnt);
}
//----------------------------------------------------------
void ofScale(ofVec3f Amnt){
	ofScale(Amnt.x, Amnt.y, Amnt.z);
}

//--------------------------------------------------------------
void ofDrawBox(const ofPoint& position, const ofPoint& size) {
	ofDrawBox( position.x, position.y, position.z, size.x, size.y, 0);
}





