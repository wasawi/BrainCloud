#pragma once

#include "ofMain.h"

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
		void setGuiScrollingBar(float xInit, float yInit, float CanvasW, float CanvasH, bool bsnap);

		void guiEvent(ofxUIEventArgs &e);
		void exit();
	
	//add contends methods
		void adjustContendstoGui(bool bsnap);
		void addTwitterContend(ofImage img, int dim, int WidgetW, std::string nameuser, string myText, bool _bsnap);
	
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
		ofxUIScrollableSliderCanvas *gui; //mine
		bool bdrawPadding, bdrawGrid;
		int gui_sizeHContent;
		//vector<tweet3d> selectedTweets;
};

