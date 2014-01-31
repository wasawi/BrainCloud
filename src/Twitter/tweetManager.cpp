#include "tweetManager.h"

//--------------------------------------------------------------
void tweetManager::setup(string xmlfilename){

	setupTwitter();
	//set all twitter users in one
	twitterClient.startQuery("@evabelmonte"); // BrainNetViz // cat

	//postTweet();
}


//--------------------------------------------------------------
void tweetManager::update(){
	
}

//--------------------------------------------------------------
void tweetManager::draw(){
	
	drawQueryTwitters();
	twitterClient.drawdebug();
}


//--------------------------------------------------------------
void tweetManager::cleanImgUsers( std::vector <ofImage> & a ) {    
 	cout << "cleanImgUsers size=" << a.size() << endl;
    a.clear(); 
	cout << "cleanImgUsers Done size=" << a.size() << endl;
}


//--------------------------------------------------------------
void tweetManager::setAllQueryTwittersAtGui(){
	
	float dim = 50;
	float xInit		= 4;
	float CanvasW   = 550;
	float ScrollW	= 40;
	float WidgetW = CanvasW -ScrollW - (xInit * 4);
	bool bsnap = true;
	
	cout << "GetTotalLoadedTweets=" << twitterClient.getTotalLoadedTweets() << endl;
	
	//Clean imgUsers vector and ask again all images
	//cleanImgUsers(imgUsers);

	
	for(int i=0; i< twitterClient.getTotalLoadedTweets()-2; i++){
		
		cout << "Start tweet=" << i << endl;
		
		tweet = twitterClient.getTweetByIndex(i);
		
		cout << " default? " << tweet.user.default_profile << endl;
		cout << " defaultname? " << tweet.user.default_profile_image << endl;
		cout << " geo_enabled? " << tweet.user.geo_enabled << endl;
		
		if ( /*tweet.user.geo_enabled*/ true ) {
		
			//Load Contend	Tweets
			//img = new ofImage();
			//img->loadImage("images/bikers.jpg");
			//string nameuser = tweet.user.screen_name;
			//string myText = "\"It's a little-acknowledged fact, yet an unanswerable one, that states exist in great part to maintain a monopoly on violence\" - Deborah Orr";
			
			bool validinfo1 = false;
			bool validinfo2 = false;
			
			string nameuser;
			string myText;
			
			cout << "user start" << endl;
			if ( sizeof(tweet.user.screen_name) ){
				nameuser = tweet.user.screen_name;
				validinfo1 = true;
			}
			cout << "user end= " << nameuser <<  endl;
			cout << "myText start" << endl;
			
			if ( sizeof(tweet.text) ){
				myText = tweet.text;
				validinfo2 = true;
			}
			cout << "myText end = " << myText << endl;
			
			if(tweet.isProfileImageLoaded()) {
				
				//TODO That way to destroy Img and create againg could be slower, try to reserve fisrt some locations
				//cout << "Image uSer pointer[" << i << "]" <<  &tweet.user.profile_image << endl;
				//cout << "imgUsers[" << i << "]" <<  &(imgUsers[i])<< endl;
				
				imgUsers.push_back(ofImage());
				imgUsers.back().clone(tweet.user.profile_image);
				
				if( validinfo1 && validinfo2 ){
					
					cout << "Go to Added contend" << endl;
					guiManager::getInstance()->addTwitterContend(imgUsers.back(), dim, WidgetW, nameuser, myText, bsnap);
					cout << "Added contend" << endl;
				}
			}
	
			cout << "End tweet=" << i << "validinfos = " << validinfo1 << ":" << validinfo2 << "sizes= " << tweet.text.length() << ":" << tweet.user.screen_name.length() << endl;
			
		}
		
		
	}
	
	//guiManager::getInstance()->adjustContendstoGui(bsnap);
	
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
       // twitterClient.startQuery("cat");
		setAllQueryTwittersAtGui();
    }
	if(key == 'w') {
		twitterClient.getTwitterMessages("gats", "100"); //  old way to ask to twitter
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

