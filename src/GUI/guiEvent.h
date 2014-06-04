//
//  guiEvent.h
//  NeuroTwitter
//
//  Created by Jordi on 12/02/14.
//
//
#pragma once
#include "ofMain.h"

class guiEvent : public ofEventArgs {
public:
	
    string  message;
	int		value;

	// my events
    static ofEvent <guiEvent> newSearch;
	static ofEvent <guiEvent> tweetReceived;
	static ofEvent <guiEvent> tabSelected;
//	static ofEvent <guiEvent> postTweet;
};

