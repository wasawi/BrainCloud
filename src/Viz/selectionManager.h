//
//  selectionManager.h
//  NeuroTwitter
//
//  Created by Jordi on 01/04/14.
//
//

#pragma once
#include "ofMain.h"


struct sphereSelection{
	float	radius;
	ofVec3f	position;
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

	void drawSphereAxis(ofVec3f position=ofVec3f(0), float radius=20, float stripWidth = 10, int circleRes = 60);

	//getters
	
	vector<sphereSelection> getSelection();
	
protected:
private:
	
	struct voxel{
		int		index;
		ofVec3f	position;
	};
	
	string myFilePath;
	vector<voxel> voxels;
	vector<sphereSelection> spheres;
	
	//selection
	ofSpherePrimitive selectionSphere;

};