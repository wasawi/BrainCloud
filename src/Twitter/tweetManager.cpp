#include "tweetManager.h"

//----------------------------------------------
tweetManager::tweetManager()
{
}
//----------------------------------------------
tweetManager::~tweetManager(){
}

//--------------------------------------------------------------
void tweetManager::setup(string CONSUMER_KEY, string CONSUMER_SECRET){
	
    twitterClient.setDiskCache(true);
    twitterClient.setAutoLoadImages(true, false); // Loads images into memory as ofImage;
	
	twitterClient.authorize(CONSUMER_KEY, CONSUMER_SECRET);
	actualTweet = 0;
	
	// listen to new searches
    ofAddListener(guiEvent::newSearch, this, &tweetManager::searchQuery);
	
	// listen when tweets are received
//    ofAddListener(guiEvent::newSearch, this, &tweetManager::searchQuery);
	
}

//--------------------------------------------------------------
void tweetManager::update(){
}

//--------------------------------------------------------------
void tweetManager::draw(){
	twitterClient.printDebugInfo();
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
void tweetManager::setAllQueryTwittersAtGui(){
	
	float dim = 50;
	float xInit		= 4;
	float CanvasW   = 550;
	float ScrollW	= 40;
	float WidgetW = CanvasW -ScrollW - (xInit * 4);
	bool  bsnap = true;
	
	if(ofGetLogLevel()== OF_LOG_VERBOSE)cout << "GetTotalLoadedTweets =" << twitterClient.getTotalLoadedTweets() << endl;
	
	//Clean imgUsers vector and ask again all images
	cleanImgUsers(imgUsers);

	
	for(int i=0; i< twitterClient.getTotalLoadedTweets(); i++){
		
		ofLogVerbose("tweetManager ") << "Start tweet ************************************************";
		tweet = twitterClient.getTweetByIndex(i);
		
//		ofLogVerbose("tweetManager ") << "default? " << tweet.user.default_profile << endl;
//		ofLogVerbose("tweetManager ") << "defaultname? " << tweet.user.default_profile_image << endl;
//		ofLogVerbose("tweetManager ") << "geo_enabled? " << tweet.user.geo_enabled << endl;
		
		if ( /* here we will check if the tweet has brain coordinates*/ true ) {
		
			//Load Content	Tweets
			//img = new ofImage();
			//img->loadImage("images/bikers.jpg");
			//string nameuser = tweet.user.screen_name;
			//string myText = "\"It's a little-acknowledged fact, yet an unanswerable one, that states exist in great part to maintain a monopoly on violence\" - Deborah Orr";
			
			bool validinfo1 = false;
			bool validinfo2 = false;
			
			string screen_name;
			string tweetText;
			
			if ( sizeof(tweet.user.screen_name) ){
				screen_name = tweet.user.screen_name;
				validinfo1 = true;
			}

			if ( sizeof(tweet.text) ){
				tweetText = tweet.text;
				validinfo2 = true;
			}
			ofLogVerbose("tweetManager ") << "tweetText = " << tweetText;
			
			if(tweet.isProfileImageLoaded()) {
				
				imgUsers.push_back(ofImage());
				imgUsers.back().clone(tweet.user.profile_image);
				if( validinfo1 && validinfo2 ){
					guiManager::getInstance()->addTwitterContent(imgUsers.back(),
																 tweet.user.name,
																 tweet.user.screen_name,
																 tweet.text);
				}
			}
			ofLogVerbose("tweetManager ") << "End tweet=" << i << "validinfos = " << validinfo1 << ":" << validinfo2 << "sizes= " << tweet.text.length() << ":" << tweet.user.screen_name.length() << endl;
			
		}
	}
}

//--------------------------------------------------------------
void tweetManager::keyReleased(int key){
    
    if(key == 'q') {
		setAllQueryTwittersAtGui();
    }
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
	
    twitterClient.printDebugInfo();
}