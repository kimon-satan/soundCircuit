/*
 *  utils.cpp
 *  soundCircuit2
 *
 *  Created by Simon Katan on 10/03/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */


#include "utils.h"

ofVec2f utils::moduloPoint(ofVec2f t_point, ofVec2f t_dims){
	
	ofVec3f point(t_point.x, t_point.y, 0);
	point = moduloPoint(point, t_dims);
	
	return point;
	
}

ofVec3f utils::moduloPoint(ofVec3f t_point, ofVec2f t_dims){
	
	ofVec3f point(t_point);
	
	if(t_point.x < -t_dims.x/2){
		point.set(t_point.x + t_dims.x, t_point.y, t_point.z);
	}else if(t_point.x > t_dims.x/2){
		point.set(t_point.x - t_dims.x, t_point.y, t_point.z);
	}
	
	if(t_point.y < -t_dims.y/2){
		point.set(t_point.x, t_point.y + t_dims.y, t_point.z);
	}else if(t_point.y > t_dims.y/2){
		point.set(t_point.x, t_point.y - t_dims.y, t_point.z);
	}
	
	return point;
}
