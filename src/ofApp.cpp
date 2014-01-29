#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{	
	// OF vars
	ofSetLogLevel(OF_LOG_VERBOSE);
	//	ofSetLogLevel(OF_LOG_WARNING);
	//	ofSetLogLevel(OF_LOG_ERROR);
	ofEnableSmoothing();
	ofSetFrameRate(30);
	
	// setup Managers
	//myTwitterManager.setup();
	setupTwitter();
	//postTweet();
	myGuiManager.setup();
//	myVizManager.setup();
	
}

//--------------------------------------------------------------
void ofApp::update()
{
//	myTwitterManager.update();
//	myGuiManager.update();
//	myVizManager.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    if(client.isAuthorized())
    {
        ofBackground(100);
    }
    else
    {
        ofBackground(255,0,0);
    }
	
	myGuiManager.draw();
	
	if (bParsed) {
		ofDrawBitmapString(jsonParser.getRawString(), 10, 14);
	}
	
}

//--------------------------------------------------------------
void ofApp::setupTwitter(){
	// A simplified interface when using the ofxExampleTwitterClient class.
    // Twitter-specific configuration details can be taken care of the
    // ofxOAuth base class.
	//    client.setup("CONSUMER_KEY","CONSUMER_SECRET");
    client.setup("pHA27PLNeoFD1R3093jEQ","78025mOujCNB3aAk04TwCd6hRFvtB1gPO42DEWYYs");
	bParsed=false;
	
	if(client.isAuthorized())
	{
		
		std::string hashTag = "BrainNetViz";
		std::string count = "100";
		std::string str = client.getHashTag(hashTag, count);
		// Now parse the JSON
		bool parsingSuccessful = jsonParser.parse(str);
		if (parsingSuccessful) {
			string parsed = jsonParser.getRawString();
			ofBuffer msg(parsed.c_str(), parsed.length());
			bool fileWritten = ofBufferToFile("getTweet.json", msg);

			if(jsonParser.isMember("errors")) {
				cout << jsonParser.getRawString();
				ofDrawBitmapString(jsonParser.getRawString(), 10, 14);
			} else if(jsonParser.isArray()) {
				int n = 0;
				ofxJSONElement profile_image_url = jsonParser[n]["search_metadata"];
				for(int i = 0; i < profile_image_url.size(); i++) {
					std::string message = profile_image_url[i]["query"].asString();
					ofDrawBitmapString(message, 500, 40*i+40);
					cout << message << endl;
				}
			}
			
		} else {
			ofLogWarning("ofApp::keyPressed") << "Failed to parse JSON.";
		}
		bParsed=true;
	}
	else
	{
		ofLogWarning("ofApp::keyPressed") << "Not authorized yet.";
	}
}

//--------------------------------------------------------------
void ofApp::postTweet(){
	client.setup("pHA27PLNeoFD1R3093jEQ","78025mOujCNB3aAk04TwCd6hRFvtB1gPO42DEWYYs");
	bParsed=false;
	
	if(client.isAuthorized())
	{
		std::string Tweet = "Hello World.";
		std::string hashtag = "BrainNetViz";
		std::string str = client.postTweet(Tweet, hashtag);
		
		// Now parse the JSON
		bool parsingSuccessful = jsonParser.parse(str);
		if (parsingSuccessful) {
			string parsed = jsonParser.getRawString();
			ofBuffer msg(parsed.c_str(), parsed.length());
			bool fileWritten = ofBufferToFile("postTweet.json", msg);
			
			if(jsonParser.isMember("errors")) {
				cout << jsonParser.getRawString();
				ofDrawBitmapString(jsonParser.getRawString(), 10, 14);
			} else if(jsonParser.isArray()) {
				int n = 0;
				ofxJSONElement profile_image_url = jsonParser[n]["search_metadata"];
				for(int i = 0; i < profile_image_url.size(); i++) {
					std::string message = profile_image_url[i]["query"].asString();
					ofDrawBitmapString(message, 500, 40*i+40);
					cout << message << endl;
				}
			}
			
		} else {
			ofLogWarning("ofApp::keyPressed") << "Failed to parse JSON.";
		}
		bParsed=true;
	}
	else
	{
		ofLogWarning("ofApp::keyPressed") << "Not authorized yet.";
	}
}

//--------------------------------------------------------------
void ofApp::exit()
{
	myGuiManager.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	
	/*
    if(gui->hasKeyboardFocus())
    {
        return;
    }
	switch (key)
	{
		case '`':

			break;
			
		case 'f':
			ofToggleFullscreen();
			break;
            
		case 'h':
            gui->toggleVisible();
			break;
            
		case 'p':
			bdrawPadding = !bdrawPadding;
			gui->setDrawWidgetPaddingOutline(bdrawPadding);
			break;
            
		case '[':
			gui->setDrawWidgetPadding(false);
			break;
            
		case ']':
			gui->setDrawWidgetPadding(true);
			break;
            
		case ' ':

			break;

		default:
			break;
	}
	 */
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
















