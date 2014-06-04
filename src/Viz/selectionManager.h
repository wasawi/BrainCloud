//
//  selectionManager.h
//  NeuroTwitter
//
//  Created by Jordi on 01/04/14.
//
//

#pragma once
#include "ofMain.h"
#include "myUtils.h"
#include "ofxBox.h"

struct sphere{
	float	radius;
	ofVec3f	position;
};

enum tool
{
	BOX,
	SPHERE,
};

class selectionManager{
public:
	
	selectionManager();
	~selectionManager();
	void setup();
	void draw(ofCamera& cam);
	
	void load();
	void save();
	
	void select(ofVec3f pos, float r);
	void add();
	void clear();

	void drawAxis();
	void drawBoxAxis();
	void drawSphereAxis();

	//setters
	void setCurrentTool(tool newTool);
	
	//getters
	vector<ofxBox> getBoxes();
	vector<sphere> getSpheres();

	
protected:
private:
		
	string myFilePath;
	tool currentTool;
	
	// selection objects
	vector<ofxBox> boxes;
	vector<sphere> spheres;
	
	//selection
	ofSpherePrimitive selectionSphere;
	ofxBox selectionBox;
};