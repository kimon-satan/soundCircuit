/*
 *  belch.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 24/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef belch_h
#define belch_h

#include "baseBlipDraw.h"

class belch : public baseBlipDraw {
	
public:
	
	belch();
	void setup(ofVec2f t_dims, baseBlipDraw * t_draw);
	void update();
	void draw(int t_wrap = 0);
	
	float getOffset(int t);
	static vector<paramAttributes>getParamDefs();
	
private:
	
	ofVec2f p1,p2,b1,b2,bl1,bl2;
	float height, width;
	float offset[4];
	ofRectangle blankRect;
};


#endif