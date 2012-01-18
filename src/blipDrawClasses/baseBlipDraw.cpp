/*
 *  baseBlipDraw.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 28/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "baseBlipDraw.h"

//getters and setters

void baseBlipDraw::setTimeParams(bool & t_active, float & t_envVal){

	isActive = t_active;
	envVal = t_envVal;

	
}

void baseBlipDraw::setBlipParams(ofVec2f & t_dir, ofVec2f & t_sp, ofVec2f & t_ep, float & t_l){

	direction = t_dir;
	startPos = t_sp;
	endPos = t_ep;
	length = t_l;
	

}

void baseBlipDraw::setPresetParams( vector<float> t_params){

	params = t_params;
	
}

