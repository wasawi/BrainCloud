/*
 *  guiManager.h
 *  brainviztwitter
 *
 *  Created by Carles on 27/01/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

#include "ofxTwitter.h"
#include "ofxUI.h"

class guiManager {

	public:

		void setup();
		void update();
		void draw();
		
		void updateSlider();
		void setGuiScrollingBar(string scrollBarname, float dim, float xInit, float yInit, float CanvasW, float CanvasH, float ScrollW, float WidgetW, bool bsnap);

		void guiEvent(ofxUIEventArgs &e);
		void exit();
	
	//vector<tweet3d> getTweets(); 

		int id;
		string queryused;
		string userid;
		string message;
		ofVec3f location3d;
	
	//TEST DATA TO LOAD
	string myText;
    ofImage *img;
    ofFilePath imgFile;

	private:
		ofxUIScrollableSliderCanvas *gui; //mine
		bool bdrawPadding, bdrawGrid;
		int gui_sizeHContent;
		//vector<tweet3d> selectedTweets;
};

