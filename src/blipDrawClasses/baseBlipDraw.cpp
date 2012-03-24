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

	for(int i = 0; i < 2; i++){
		corners.push_back(ofVec2f(0,0));
	}
	
	envVal = 0;
	postVal = 0;
}

void baseBlipDraw::setup(ofVec2f t_dims){
	
	world_dims = t_dims;
	ofVec2f l_dir(length, length);
	l_dir *= direction;
	centre = startPos + l_dir/2;
	angle = (direction.y > 0) ? 90 : 0;
	postVal = 0;
	envVal = 0;
	
}


void baseBlipDraw::setup(ofVec2f t_dims, baseBlipDraw * t_draw){
	
	direction = t_draw->getDirection();
	startPos = t_draw->getStartPos();
	endPos = t_draw->getEndPos();
	length = t_draw->getLength();
	setup(t_dims);
	
}

//getters and setters

void baseBlipDraw::setCorners(vector<ofVec2f> & t_corners, ofVec2f t_centre, ofVec2f t_dims, float t_angle){
	
	t_corners[0] = t_centre - t_dims; //maybe a function for this in baseBlip
	t_corners[1] = t_centre + t_dims;
	t_corners[0].rotate(t_angle, t_centre);
	t_corners[1].rotate(t_angle, t_centre);
	
}

void baseBlipDraw::setWrapData(ofVec2f t_dims, float t_angle){
	
	setCorners(corners, centre, t_dims, t_angle);

}

void baseBlipDraw::setBoundingRect(vector<ofVec2f> const & points, ofVec2f t_centre, float t_angle){
	
	for(int i = 0; i < 2; i++)corners[i].set(points[0]);

	for(int i= 1; i < points.size(); i++){
		
		if(points[i].x < corners[0].x)corners[0].x = points[i].x;
		if(points[i].y < corners[0].y)corners[0].y = points[i].y;
		if(points[i].x > corners[1].x)corners[1].x = points[i].x;
		if(points[i].y > corners[1].y)corners[1].y = points[i].y;
	
	}
	
	for(int i = 0; i < 2; i++){
		corners[i] += t_centre;
		corners[i].rotate(t_angle, t_centre);
	}
	

}



void baseBlipDraw::setTimeParams(bool  t_active, float  t_envVal, float  t_postVal){

	isActive = t_active;
	envVal = t_envVal;
	postVal = t_postVal;
	
}


void baseBlipDraw::setBlipParams(ofVec2f & t_dir, ofVec2f & t_sp, ofVec2f & t_ep, float & t_l){

	direction = t_dir;
	startPos = t_sp;
	endPos = t_ep;
	length = t_l;

}

void baseBlipDraw::setPresetParams( vector<float> t_params){params = t_params;}
vector<ofVec2f> baseBlipDraw::getCorners(){return corners;}

ofVec2f baseBlipDraw::getDirection(){return direction;}
ofVec2f baseBlipDraw::getStartPos(){return startPos;}
ofVec2f baseBlipDraw::getEndPos(){return endPos;}
float baseBlipDraw::getLength(){return length;}

void baseBlipDraw::setDecayRatio(float decay, float postdecay){
	
	decayRatio = decay/(decay+postdecay);
	
}
