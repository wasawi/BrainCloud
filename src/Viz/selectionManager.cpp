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
//		ofTranslate(selection[i].getPosition());
		ofSetColor(255,40);
		ofSpherePrimitive sphere;
		
		sphere.setPosition(spheres[i].position);
		sphere.setRadius(spheres[i].radius);
		sphere.draw();
//		cout <<"pos "<< selection[i].getPosition()<< endl;
		
		
/*		ofCylinderPrimitive cylinder;
		cylinder.setResolution(60, 1);
		ofVec3f	camPos		= cam.getPosition();
		cylinder.lookAt(camPos);
		ofSetColor(ofColor::white);
		cylinder.setPosition(spheres[i].position);
		cylinder.setRadius(spheres[i].radius);
		cylinder.draw(OF_MESH_WIREFRAME);
*/
	}
	
	
	//cout <<"pos"<< selectionSphere.getPosition()<< endl;
	
	/*
	ofVec3f pos =cam.worldToScreen(selectionSphere.getPosition()*volSize);
	float r = selectionSphere.getRadius();
	
	ofPushStyle();
	ofSetColor(255,50);
	ofCircle(pos, r * volSize.x);
	ofSetColor(ofColor::white);
	ofNoFill();
	ofCircle(pos, r * volSize.x);
	ofPopStyle();
	 */
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
	
	sphereAttr sphere;
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
void selectionManager::drawSphereAxis(ofCamera& cam, ofVec3f position, float radius, float stripWidth, int circleRes){
	
//	selectionSphere.draw(OF_MESH_WIREFRAME);
	
	//	ofRotate
	//	cam.get
	//	ofCircle(0, 0, 0, radius);
	
	ofPushMatrix();
//		ofVec3f camOrientation = cam.getOrientationEuler();
		ofVec3f	camPos		= cam.getPosition();
//		ofVec3f camupDir	= cam.getUpDir();

		ofSetColor(ofColor::white);
		ofCircle(0, 0, 0, radius);
	ofPopMatrix();
	 



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








