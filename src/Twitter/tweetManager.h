#pragma once

#include "ofMain.h"
#include "ofxTwitter.h"
#include "ofxXmlSettings.h"
#include "ofxTwitter3d.h"


#include "ofxExampleTwitterClient.h"
#include "ofxJSONElement.h"


class tweetManager {
	
	public:

		void setup(string xmlfilename);
		void update();
		void draw();
		
		vector<tweet3d> getTweets();
	
		//	ofxOAuth
		ofxExampleTwitterClient client;
		
		//	ofxJSON
		ofxJSONElement jsonParser;
		bool bParsed;
		
		//	Twitter
		void setupTwitter();
		void postTweet();
	
	private:
	
		ofxTwitter3d mytwitter3d;
		vector<tweet3d> alltweets;
		vector<tweet3d> selectedTweets;
		
		ofxXmlSettings xmltwitter;
		


};