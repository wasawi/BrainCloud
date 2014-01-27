/*
 *  vizManager.h
 *  brainviztwitter
 *
 *  Created by Carles on 27/01/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "tweet3d.h"

class vizManager {
	
public:
	
	void setup();
	void update();
	void draw();
	
	tweet3d  searchTweetByLocation();
	
private:
	vector<tweet3d> alltweets;
    vector<tweet3d> selectedTweets;
};