/*
 *  flipper.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 13/02/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef flipper_h
#define flipper_h

#include "baseBlipDraw.h"

class flipper : public baseBlipDraw {
	
public:
	
	flipper();
	void update();
	void draw(int t_wrap = 0);
	static vector<paramAttributes>getParamDefs();
	
private:
	
	float alpha;
	float pivot[3];
	ofColor c;
	ofRectangle testRect;
	bool isDecay;
	
	
};


#endif