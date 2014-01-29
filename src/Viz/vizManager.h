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