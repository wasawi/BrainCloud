/*
 *  tweetManager.h
 *  brainviztwitter
 *
 *  Created by Carles on 27/01/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxTwitter.h"
#include "ofxXmlSettings.h"
#include "ofxTwitter3d.h"

class tweetManager {
	
	public:

		void setup(string xmlfilename);
		void update();
		void draw();
		
		vector<tweet3d> getTweets();
	
	private:
	
		ofxTwitter3d mytwitter3d;
		vector<tweet3d> alltweets;
		vector<tweet3d> selectedTweets;
		
		ofxXmlSettings xmltwitter;
		


};