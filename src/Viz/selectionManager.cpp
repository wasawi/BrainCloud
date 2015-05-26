//
//  selectionManager.cpp
//  NeuroTwitter
//
//  Created by Jordi on 01/04/14.
//
//

#include "selectionManager.h"

selectionManager::selectionManager(){
	currentTool = SPHERE;
//	currentTool = BOX;
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

	
	if (currentTool == BOX){
		for( int i = 0; i < boxes.size(); i++){
			
			drawBox(boxes[i].position, boxes[i].size.x);
		}
	}else{

		for( int i = 0; i < spheres.size(); i++){

			// We need to orient the shphere depending on the position of the camera
			// It will b better to ge the actual camera position instead of passing it as argument.
			// this would be better:
			//	ofCamera cam = ofGetCurrentCamera();
			
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

			drawSphere(ofVec3f(0), spheres[i].radius);

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
		drawBoxAxis(selectionBox.getPosition(), selectionBox.getWidth());
	}else{
		drawSphereAxis(selectionSphere.getPosition(), selectionSphere.getRadius());
	}
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







