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

    static ofEvent <guiEvent> newSearch;
};

