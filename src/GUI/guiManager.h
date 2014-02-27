#pragma once

#include "ofMain.h"
#include "tweetManager.h"
#include "ofxTwitter.h"
#include "ofxUI.h"
#include "guiEvent.h"
#include "myUtils.h"

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
	void update();
	void draw();

	// My public methods
	void addTwitterContent(ofImage img, string name ,string user_name, string tweetText);
	
private:
	
	// Load GUIs
	void setupTabBar();
	void updateSlider();
	void setupScrollCanvas();
	void setupSearchCanvas();
	void setupPostCanvas();
	void changeTabBar();
	
	// Canvas
	ofxUICanvas *tabCanvas;
	ofxUICanvas *postCanvas;
	ofxUICanvas *textInputCanvas;
	ofxUIScrollableSliderCanvas *scrollCanvas;

	// Events
	void tabCanvasEvent(ofxUIEventArgs &e);
	void scrollCanvasEvent(ofxUIEventArgs &e);
	void textInputEvent(ofxUIEventArgs &e);
	void postCanvasEvent(ofxUIEventArgs &e);

	// Tab
	enum tabSelector
	{
		SEARCH,
		POST,
		FILTER
	};
	tabSelector myTabselector;
	bool searchToggle;
	bool postToggle;
	
	//vector<tweet3d> selectedTweets;
	void adjustContentstoGui(bool bsnap);
	
	// tabCanvas
	float tabCanvasX;
	float tabCanvasY;
	float tabCanvasW;
	float tabCanvasH;
	float toggleW;
	float toggleH;

	// postCanvas
	float postCanvasX;
	float postCanvasY;
	float postCanvasW;
	float postCanvasH;
	float postFieldH;
	
	// tweetsCanvas
	float tweetsCanvasX;
	float tweetsCanvasY;
	float tweetsCanvasW;
	float tweetsCanvasH;

	// searchCanvas
	float searchCanvasX;
	float searchCanvasY;
	float searchCanvasW;
	float searchCanvasH;
	
	// search field
	float textInputY;
	float searchFieldX;
	float searchFieldY;
	float searchFieldW;
	float searchFieldH;

	bool bsnap;
	float space;
	float dim;
	float WidgetW;
	float sliderW;
	
	int	nResponses;
	float nResponsesX;
	float nResponsesY;
	float nResponsesW;
};


// ???
/*	int id;
 string queryused;
 string userid;
 string message;
 ofVec3f location3d;
 
 //TEST DATA TO LOAD
 string myText;
 ofImage *_img;
 ofFilePath imgFile;
 */
