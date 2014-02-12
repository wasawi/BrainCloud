#pragma once

#include "ofMain.h"
#include "ofxTwitter.h"
#include "ofxXmlSettings.h"
#include "ofxTwitter3d.h"
#include "guiManager.h"

class tweetManager {
	
private:
    static bool	instanceFlag;
    static tweetManager *single;
	
public:
	tweetManager();
	~tweetManager();
	
	static tweetManager* getInstance();

	void setup(	string CONSUMER_KEY, string CONSUMER_SECRET);
	void update();
	void draw();
	void keyReleased(int key);
	
	void searchQuery(string s);
	void setupTwitter();
	void drawQueryTwitters();
	void setAllQueryTwittersAtGui();
	void cleanImgUsers( std::vector <ofImage> & a );

		
	//query eaxmple
		ofxTwitter twitterClient;
		ofxTwitterTweet tweet;
		int actualTweet;
		bool reloadTweet;		
		ofImage userImage;
		vector<ofImage> imgUsers;
	
private:
		ofxTwitter3d mytwitter3d;
		vector<tweet3d> alltweets;
		vector<tweet3d> selectedTweets;
		
		ofxXmlSettings xmltwitter;

	string screen_name;
	string myText;
	string name;
	
};