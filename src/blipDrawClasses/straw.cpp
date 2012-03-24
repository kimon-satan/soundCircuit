/*
 *  straw.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 17/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "straw.h"

straw::straw():baseBlipDraw(){
	
	
}

void straw::update(){
	
	pivot = params[0];
	p_dim = params[1];
	
	
	
	float swing = (postVal > 0) ? params[2] * 0.75 * envVal + params[2] * 0.25 * postVal: params[2] * envVal;
	
	if(isActive){
		pivot += ofRandom(-swing, swing);
	}
	
	
	vector<ofVec2f> points;
	points.push_back(ofVec2f(-params[3]/2,-p_dim - (p_dim * params[5])));
	points.push_back(ofVec2f(params[3]/2,p_dim - (p_dim * params[5])));
	
	for(int i = 0; i < 2; i++)points[i].rotate(pivot);
	
	setBoundingRect(points, centre, angle);
	

	
}

void straw::draw(int t_wrap){
	
	
	glPushMatrix();
	
	glTranslatef(centre.x, centre.y, 0);
	
	glRotatef(angle, 0, 0, 1);
	
	float offset = p_dim * params[5];
	
	ofPushStyle();
	ofSetColor(params[4]);
	ofEnableSmoothing();
	ofSetLineWidth(params[3]);
	glRotatef(pivot, 0, 0, 1);
	ofLine(0, -p_dim - offset, 0, p_dim - offset);   //replace lines with rectangles 
	ofDisableSmoothing();
	ofPopStyle();
	
	
	glPopMatrix();
	
	
}

vector<paramAttributes> straw::getParamDefs(){
	
	vector<paramAttributes> def;
	
	paramAttributes rotation, height, pivot, thickness, grayCol, trackOffset;
	
	rotation.name = "rotation"; rotation.min_val = -25; rotation.max_val = 25; rotation.abs_value = 0;
	pivot.name = "pivot"; pivot.min_val = 10; pivot.max_val = 45; pivot.abs_value = 30; 
	height.name = "height"; height.min_val = 20 * WORLD_UNIT; height.max_val = 300 * WORLD_UNIT; height.abs_value = 50 * WORLD_UNIT;
	thickness.name = "thickness"; thickness.min_val = 1; thickness.max_val = 10; thickness.abs_value = 1;
	grayCol.name = "grayCol"; grayCol.min_val = 0; grayCol.max_val = 150; grayCol.abs_value = 50;
	trackOffset.name = "trackOffset"; trackOffset.min_val = -1; trackOffset.max_val = 1; trackOffset.abs_value = 0; //5
	
	def.push_back(rotation);
	def.push_back(height);
	def.push_back(pivot);
	def.push_back(thickness);
	def.push_back(grayCol);
	def.push_back(trackOffset);
	
	return def;
	
}
