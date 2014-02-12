#pragma once

#include "ofMain.h"
#include "tweetManager.h"
#include "ofxTwitter.h"
#include "ofxUI.h"
#include "guiEvent.h"

class guiManager {

// variables & methods for singleton
private:
    static bool	instanceFlag;
    static guiManager *single;
	
public:
	static guiManager* getInstance();
// end singleton

	
public:	
	guiManager();
	~guiManager();
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	void updateSlider();
	void setupScrollCanvas();
	void setupSearchInput();
	void scrollCanvasEvent(ofxUIEventArgs &e);
	void textInputEvent(ofxUIEventArgs &e);
	
	//add contents methods
	void adjustContentstoGui(bool bsnap);
	void addTwitterContent(ofImage img, string name ,string user_name, string tweetText);

	//vector<tweet3d> getTweets();
	
	// Events
//	ofEvent <string> newSearch;

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
	
	float canvasX;
	float canvasY;
	float canvasW;
	float canvasH;
	bool bsnap;
	float space;
	
	float textInputY;
	float searchFieldW;
	float searchFieldH;
	
	float dim;
	float WidgetW;
	float sliderW;
	
	int	nResponses;
	float nResponsesX;
	float nResponsesW;
};

