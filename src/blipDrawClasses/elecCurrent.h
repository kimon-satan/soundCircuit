/*
 *  elecCurrent.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 17/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef elecCurrent_h
#define elecCurrent_h

#include "baseBlipDraw.h"

class elecCurrent : public baseBlipDraw {
	
public:
	
	elecCurrent();
	void update();
	void draw(bool isWrapped = false);
	
private:
	
	ofRectangle blankRect;
	float waveHeight, max_waveHeight, density;
	vector<ofVec2f> peaks;
	
};


#endif