/*
 *  utils.cpp
 *  soundCircuit2
 *
 *  Created by Simon Katan on 10/03/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */


#include "utils.h"
#include "blip.h"

bool blipIndex::operator()(const blip &t_blip, const int &t_index)const{
	return t_blip.getIndex() == t_index;
}

bool segmentIndex::operator()(const segment &t_segment, const int &t_index)const{
	return t_segment.getIndex() == t_index;
}

ofVec2f utils::moduloPoint(ofVec2f t_point, ofVec2f t_dims){
	
	ofVec3f point(t_point.x, t_point.y, 0);
	point = moduloPoint(point, t_dims);
	
	return ofVec2f(point.x, point.y);
	
}

ofVec3f utils::moduloPoint(ofVec3f t_point, ofVec2f t_dims){
	
	ofVec3f point(t_point);
	
	if(t_point.x < -t_dims.x/2){
		point.set(point.x + t_dims.x, point.y, point.z);
	}else if(t_point.x > t_dims.x/2){
		point.set(point.x - t_dims.x, point.y, point.z);
	}
	
	if(t_point.y < -t_dims.y/2){
		point.set(point.x, point.y + t_dims.y, point.z);
	}else if(t_point.y > t_dims.y/2){
		point.set(point.x, point.y - t_dims.y, point.z);
	}
	
	return point;
}

int utils::round(float x)
{
    return static_cast<int>(floor(x + 0.5f));
}
