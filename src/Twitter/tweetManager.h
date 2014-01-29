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
	void keyReleased(int key);
		
	void drawQueryTwitters();
	void setupTwitter();
		
	//query eaxmple
		ofxTwitter twitterClient;
		ofxTwitterTweet tweet;
		int actualTweet;
		bool reloadTweet;		
		ofImage userImage;
	
	private:
	
		ofxTwitter3d mytwitter3d;
		vector<tweet3d> alltweets;
		vector<tweet3d> selectedTweets;
		
		ofxXmlSettings xmltwitter;
		


};