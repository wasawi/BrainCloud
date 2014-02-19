#include "tweetManager.h"

//----------------------------------------------
tweetManager::tweetManager(){

}

//----------------------------------------------
tweetManager::~tweetManager(){
}

//--------------------------------------------------------------
void tweetManager::setup(string CONSUMER_KEY, string CONSUMER_SECRET){
	
    twitterClient.setDiskCache(true);
    twitterClient.setAutoLoadImages(true, false); // Loads images into memory as ofImage;
	
	twitterClient.authorize(CONSUMER_KEY, CONSUMER_SECRET);
//	actualTweet = 0;
	
	// listen to new searches. maybe we will need to place it somewhere else and use removelistener
    ofAddListener(guiEvent::newSearch, this, &tweetManager::searchQuery);

	// listen when tweets are received
	ofAddListener(guiEvent::gotTweet, this, &tweetManager::setOneTweetToGui);
	
}

//--------------------------------------------------------------
void tweetManager::searchQuery(guiEvent &e) {
	twitterClient.startQuery(e.message, e.value);
}

//--------------------------------------------------------------
void tweetManager::cleanImgUsers( std::vector <ofImage> & a ) {
    a.clear(); 
}

//--------------------------------------------------------------
void tweetManager::setOneTweetToGui(guiEvent &e){
	
	int i = e.value;
	tweet = twitterClient.getTweetByIndex(i);
	
	ofLogVerbose("tweetManager ") << "Start tweet ************************************************";
	ofLogVerbose("tweetManager ") << "tweet.user.screen_name = " << tweet.user.screen_name;
	ofLogVerbose("tweetManager ") << "tweet.user.name = " << tweet.user.name;
	ofLogVerbose("tweetManager ") << "tweetText = " << tweet.text;
	
	if ( /* here we will check if the tweet has brain coordinates*/ true ) {
		
		if(tweet.isProfileImageLoaded()) {
			guiManager::getInstance()->addTwitterContent(tweet.user.profile_image,
															 tweet.user.name,
															 tweet.user.screen_name,
															 tweet.text);
			
		}		
	}
}



//--------------------------------------------------------------
void tweetManager::update(){
}

//--------------------------------------------------------------
void tweetManager::draw(){
}

//--------------------------------------------------------------
void tweetManager::keyReleased(int key){
/*
    if(key == 'l') {
        twitterClient.loadCacheFile();
    }
    
    if(key == OF_KEY_UP) {
        if(actualTweet < twitterClient.getTotalLoadedTweets()-1) actualTweet += 1;
    }
    
    if(key == OF_KEY_DOWN) {
        if(actualTweet > 0) actualTweet -= 1;
    }
*/
}


//--------------------------------------------------------------
void tweetManager::drawQueryTwitters(){
	// Print tweets:
	/*
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
	 //cout << "Image uSer pointer" <<  &tweet.user.profile_image << endl;
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
	 */
	
//    twitterClient.printDebugInfo();
}