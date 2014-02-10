#pragma once

#include "ofMain.h"
#include "tweetManager.h"
#include "ofxTwitter.h"
#include "ofxUI.h"

class guiManager {

public:	
	guiManager();
	~guiManager();
	
	// variables & methods for singleton
private:
    static bool	instanceFlag;
    static guiManager *single;	

public:
	static guiManager* getInstance();
	// end singleton
	
	public:

		void setup();
		void update();
		void draw();
		
		void updateSlider();
		void setupScrollCanvas(float xInit, float yInit, float CanvasW, float CanvasH, bool bsnap);
		void setupTextInput(float canvasX, float textInputY, float CanvasW, float CanvasH);
		void scrollCanvasEvent(ofxUIEventArgs &e);
		void textInputEvent(ofxUIEventArgs &e);

		void exit();
	
	//add contents methods
		void adjustContentstoGui(bool bsnap);
		void addTwitterContent(ofImage img, string name ,string user_name, string tweetText);
	
	//vector<tweet3d> getTweets(); 

		int id;
		string queryused;
		string userid;
		string message;
		ofVec3f location3d;
	
	//TEST DATA TO LOAD
	string myText;
    ofImage *_img;
    ofFilePath imgFile;

	private:
	
	ofxUICanvas *textInputCanvas;
	ofxUIScrollableSliderCanvas *scrollCanvas;
	bool bdrawPadding, bdrawGrid;
	int gui_sizeHContent;
	//vector<tweet3d> selectedTweets;
	
	float dim;
	float WidgetW;
	float sliderW;
	
};

