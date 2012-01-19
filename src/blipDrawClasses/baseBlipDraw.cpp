/*
 *  baseBlipDraw.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 28/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "baseBlipDraw.h"

baseBlipDraw::baseBlipDraw(){


}

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

void baseBlipDraw::setPresetParams( vector<float> t_params){params = t_params;}
void baseBlipDraw::setTestingRects(ofVec2f a, ofVec2f b, ofVec2f wa, ofVec2f wb){

	drawRect.set((a.x < b.x)? a.x:b.x, 
				 (a.y < b.y)? a.y:b.y, 
				 abs(b.x - a.x), abs(b.y - a.y));
	
	wrapDrawRect.set((wa.x < wb.x)? wa.x:wb.x, 
					 (wa.y < wb.y)? wa.y:wb.y, 
					 abs(wb.x - wa.x), abs(wb.y - wa.y));
	
	isDrawWrap = (drawRect!= wrapDrawRect)? true : false;

}
ofRectangle baseBlipDraw::getDrawRect(){return drawRect;}
ofRectangle baseBlipDraw::getWrapDrawRect(){return wrapDrawRect;}
bool baseBlipDraw::getIsDrawWrap(){return isDrawWrap;}