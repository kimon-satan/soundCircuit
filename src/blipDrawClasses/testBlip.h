/*
 *  testBlip.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 28/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef testBlip_h
#define testBlip_h

#include "baseBlipDraw.h"

class testBlip : public baseBlipDraw {
	
public:
	
	testBlip();
	void update();
	void draw(int t_wrap = 0);
	
private:
	
	float alpha;
	ofColor c;
	ofRectangle testRect;

	
};


#endif