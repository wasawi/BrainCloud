#include "tweetManager.h"

//--------------------------------------------------------------
void tweetManager::setup(string xmlfilename){

	//myTwitterManager.setup();
	setupTwitter();
	//postTweet();
}
//--------------------------------------------------------------
void tweetManager::update(){
	
}
//--------------------------------------------------------------
void tweetManager::draw(){
	
	drawQueryTwitters();
	
}

//--------------------------------------------------------------
void tweetManager::drawQueryTwitters(){
	// Print tweets:
    
    int maxLineSize = 90;
    
    if(twitterClient.getTotalLoadedTweets() > 0) {
        
        tweet = twitterClient.getTweetByIndex(actualTweet);
        
        ofSetColor(255, 255, 255, 125);
        if(tweet.isBannerImageLoaded()) {
            tweet.user.profile_banner.draw(0, 0, ofGetWidth(), ofGetHeight());
        }
        
        ofSetColor(0);
        
        ofDrawBitmapString("User:", ofVec2f(120,150));
        ofDrawBitmapStringHighlight(tweet.user.screen_name, ofVec2f(220,150));
        
        ofDrawBitmapString("Location:", ofVec2f(120,180));
        ofDrawBitmapStringHighlight(tweet.user.location, ofVec2f(220,180));
        
        ofDrawBitmapString("Descript.:", ofVec2f(120,210));
        string desc = tweet.user.description;
        for(int i=0;i<(desc.length()/maxLineSize)+1;i++) {
            ofDrawBitmapStringHighlight(desc.substr(i*maxLineSize,maxLineSize), ofVec2f(220,210+(30*i)));
        }
        
        ofDrawBitmapString("Text:", ofVec2f(120,300));
        string text = tweet.text;
        for(int i=0;i<(text.length()/maxLineSize)+1;i++) {
            ofDrawBitmapStringHighlight(text.substr(i*maxLineSize,maxLineSize), ofVec2f(220,300+(30*i)));
        }
        
        ofSetColor(255, 255, 255);
        if(tweet.isProfileImageLoaded()) {
            tweet.user.profile_image.draw(40, 150);
        }
        
        ofSetColor(0);
        string nav = "Now showing tweet: " + ofToString(actualTweet+1) + " of "+ofToString(twitterClient.getTotalLoadedTweets());
        ofDrawBitmapString(nav, ofVec2f(220,420));
        
    }
    
    ofSetColor(0);
    string info;
    info += "ofxTwitter query example:";
    info += "\nPress 'q' to get tweets containing 'cat'";
    info += "\nPress 'l' to load previous query from disk if avilable";
    info += "\nPress UP/DOWN to navigate tweets";
    ofDrawBitmapString(info, ofVec2f(20,20));
    
    twitterClient.printDebugInfo();
}

//--------------------------------------------------------------
void tweetManager::setupTwitter(){
	
    twitterClient.setDiskCache(true);
    twitterClient.setAutoLoadImages(true, false); // Loads images into memory as ofImage;
    
    string const CONSUMER_KEY = "pHA27PLNeoFD1R3093jEQ";
    string const CONSUMER_SECRET = "78025mOujCNB3aAk04TwCd6hRFvtB1gPO42DEWYYs";
    
    twitterClient.authorize(CONSUMER_KEY, CONSUMER_SECRET);
	
    actualTweet = 0;
}

//--------------------------------------------------------------
void tweetManager::keyReleased(int key){
    
    if(key == 'q') {
        twitterClient.startQuery("cat");
    }
    
    if(key == 'l') {
        twitterClient.loadCacheFile();
    }
    
    if(key == OF_KEY_UP) {
        if(actualTweet < twitterClient.getTotalLoadedTweets()-1) actualTweet += 1;
    }
    
    if(key == OF_KEY_DOWN) {
        if(actualTweet > 0) actualTweet -= 1;
    }
    
}

