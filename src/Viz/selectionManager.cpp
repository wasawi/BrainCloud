//
//  selectionManager.cpp
//  NeuroTwitter
//
//  Created by Jordi on 01/04/14.
//
//

#include "selectionManager.h"

selectionManager::selectionManager(){
	currentTool = BOX;
	ofSetCircleResolution(60);
	clear();
}
//--------------------------------------------------------------
selectionManager::~selectionManager(){
}

//----------------------------------------------
void selectionManager::setup(){
	
}

//----------------------------------------------
void selectionManager::draw(ofCamera& cam){

	// this would be better:
//	ofCamera cam = ofGetCurrentCamera();
	
	if (currentTool == BOX){
		for( int i = 0; i < boxes.size(); i++){
			ofPushMatrix();
			ofTranslate(boxes[i].position);

			ofPushStyle();
			ofSetColor(255,50);
			ofDrawBox(ofVec3f(0), boxes[i].size.x,boxes[i].size.y,boxes[i].size.z);

			ofSetColor(255);
			ofNoFill();
			ofDrawBox(ofVec3f(0), boxes[i].size.x,boxes[i].size.y,boxes[i].size.z);
			ofPopStyle();

			ofPopMatrix();
		}
	}else{

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
}

//----------------------------------------------
void selectionManager::load(){
	
}

//----------------------------------------------
void selectionManager::save(){
	
}

//----------------------------------------------
void selectionManager::select(ofVec3f pos, float r){
	if (currentTool == BOX){
		selectionBox.setPosition(pos);
		selectionBox.setSize(ofVec3f(r*2));
	}else{
		selectionSphere.setPosition(pos);
		selectionSphere.setRadius(r);
	}
}


//----------------------------------------------
void selectionManager::add(){
	
	if (currentTool == BOX){
		ofxBox b;
		b.position=selectionBox.getPosition();
		b.size=selectionBox.getSize();
		boxes.push_back(b);
	}else{
		sphere s;
		s.position=selectionSphere.getPosition();
		s.radius=selectionSphere.getRadius();
		spheres.push_back(s);
	}
}

//----------------------------------------------
void selectionManager::clear(){
	
	if (currentTool == BOX){
		for( int i = 0; i < boxes.size(); i++){
			boxes.clear();
		}
	}else{
		for( int i = 0; i < spheres.size(); i++){
			spheres.clear();
		}
	}
}


void selectionManager::drawAxis(){

	if (currentTool == BOX){
		drawBoxAxis();
	}else{
		drawSphereAxis();
	}
}

//--------------------------------------------------------------
void selectionManager::drawBoxAxis(){
	
	ofVec3f position	= selectionBox.getPosition();
	float	w			= selectionBox.getWidth();
	
	ofPushMatrix();
	ofTranslate(position);
	
	ofPushStyle();
	ofNoFill();
	
	// daw cirles
	ofPushView();
	
	// draw x axis
	ofSetColor(ofColor::red);
	ofDrawBox(ofVec3f(0), w,w,0);
	// draw y axis
	ofRotateY(90);
	ofSetColor(ofColor::green);
	ofDrawBox(ofVec3f(0), w,w,0);
	// draw z axis
	ofRotateX(90);
	ofSetColor(ofColor::blue);
	ofDrawBox(ofVec3f(0), w,w,0);
	
	ofPopView();
	
	// draw x axis
	ofSetColor(ofColor::red);
	ofLine(0, 0, 0, w/2, 0, 0);
	// draw y axis
	ofSetColor(ofColor::green);
	ofLine(0, 0, 0, 0, w/2, 0);
	// draw z axis
	ofSetColor(ofColor::blue);
	ofLine(0, 0, 0, 0, 0, w/2);
	
	ofPopStyle();
	ofPopMatrix();
}

//--------------------------------------------------------------
void selectionManager::drawSphereAxis(){
	
	ofVec3f position=selectionSphere.getPosition();
	float	radius=selectionSphere.getRadius();
	
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
vector<sphere> selectionManager::getSpheres(){
	return spheres;
}
//--------------------------------------------------------------
vector<ofxBox> selectionManager::getBoxes(){
	return boxes;
}
//--------------------------------------------------------------
void selectionManager::setCurrentTool(tool newTool){
	currentTool = newTool;
}







