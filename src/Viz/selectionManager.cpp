//
//  selectionManager.cpp
//  NeuroTwitter
//
//  Created by Jordi on 01/04/14.
//
//

#include "selectionManager.h"

selectionManager::selectionManager(){
	
}
//--------------------------------------------------------------
selectionManager::~selectionManager(){
	this->clear();
}

//----------------------------------------------
void selectionManager::setup(){
	
}

//----------------------------------------------
void selectionManager::draw(ofCamera& cam){
	ofSetCircleResolution(60);

	for( int i = 0; i < spheres.size(); i++){
		ofNode posNode;
        posNode.setGlobalPosition(spheres[i].position);
        posNode.lookAt(cam.getGlobalPosition(), cam.getUpDir());
		//        posNode.lookAt(cam.getPosition()); this will do, but the object will rotate ugly.
        ofQuaternion posQuat = posNode.getGlobalOrientation();
		
        float ang = 0;
        ofPoint vec;
        posQuat.getRotate(ang, vec);

        ofPushMatrix();
		ofTranslate(spheres[i].position);
		ofRotate(ang, vec.x, vec.y, vec.z);

        ofPushStyle();
		ofSetColor(255,50);
		ofCircle(0,0,0,spheres[i].radius);

		ofSetColor(255);
		ofNoFill();
		ofCircle(0,0,0,spheres[i].radius);
        ofPopStyle();
		
		ofPopMatrix();
	}
}

//----------------------------------------------
void selectionManager::load(){
	
}

//----------------------------------------------
void selectionManager::save(){
	
}

//----------------------------------------------
void selectionManager::select(ofVec3f pos, float r){
	selectionSphere.setPosition(pos);
	selectionSphere.setRadius(r);
}


//----------------------------------------------
void selectionManager::add(){

	ofSpherePrimitive sphereCopy;

	sphereCopy=selectionSphere;
	
	sphereSelection sphere;
	sphere.position=selectionSphere.getPosition();
	sphere.radius=selectionSphere.getRadius();
	
	spheres.push_back(sphere);

/*	cout <<"pos "<< selectionSphere.getPosition()<< endl;
	cout <<"r "<< selectionSphere.getRadius()<< endl;
	
	for( int i = 0; i < spheres.size(); i++){
		cout <<"posV "<< spheres[i].position<< endl;
		cout <<"rV "<< spheres[i].radius<< endl;
	}
 */
}

//----------------------------------------------
void selectionManager::clear(){

	for( int i = 0; i < spheres.size(); i++){
		spheres.clear();
	}

//	voxels.clear();
}


//--------------------------------------------------------------
void selectionManager::drawSphereAxis(ofVec3f position, float radius, float stripWidth, int circleRes){
	
	position=selectionSphere.getPosition();
	radius=selectionSphere.getRadius();

	
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
vector<sphereSelection> selectionManager::getSelection(){

	return spheres;
}







