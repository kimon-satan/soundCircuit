/*
 *  utils.h
 *  soundCircuit2
 *
 *  Created by Simon Katan on 08/03/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef utils_h
#define utils_h

#define WORLD_UNIT 1
#define MIN_TRACK 30
#define M_VAL 250

#include "ofMain.h"

class utils{

public:
	
	static ofVec2f moduloPoint(ofVec2f t_point, ofVec2f t_dims);
	static ofVec3f moduloPoint(ofVec3f t_point, ofVec2f t_dims);
	

};

#endif