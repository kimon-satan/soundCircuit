/*
 *  straw.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 17/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef straw_h
#define straw_h

#include "baseBlipDraw.h"

class straw : public baseBlipDraw {
	
public:
	
	straw();
	void update();
	void draw(int t_wrap = 0);
	static vector<paramAttributes>getParamDefs();
	
private:
	
	float pivot;
	float p_dim;
	
	
};


#endif