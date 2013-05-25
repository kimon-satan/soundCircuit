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

class blip;
class segment;

//search predicate for blip
struct blipIndex:public binary_function<blip, int, bool> {
	
	bool operator()(const blip &t_blip, const int &t_index)const;
	
};

//search predicate for segment
struct segmentIndex:public binary_function<segment, int, bool> {
	
	bool operator()(const segment &t_segment, const int &t_index)const;
	
};

class utils{

public:
	
	static ofVec2f moduloPoint(ofVec2f t_point, ofVec2f t_dims);
	static ofVec3f moduloPoint(ofVec3f t_point, ofVec2f t_dims);
    static int round(float x);
	

};

#endif