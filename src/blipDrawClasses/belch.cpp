/*
 *  belch.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 24/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "belch.h"

belch::belch():baseBlipDraw(){


	for(int i = 0; i < 4; i++)offset[i] = ofRandom(1,1.9);
	//make thin for high frequencies fat for low freqs 
	//amplitude is height

}

void belch::setup(ofVec2f t_dims, baseBlipDraw * t_draw){

	baseBlipDraw::setup(t_dims,t_draw);
	
	belch * t = (belch *)t_draw;
	for(int i = 0; i < 4; i++)offset[i] = t->getOffset(i);

}

void belch::update(){

	p1.set(-length * direction/2);
	p2.set(length * direction/2);
	height = params[0]/3;
	width = length/2;
	
	blankRect.setFromCenter(0, 0, length -2, 2);
	
	float swing = (postVal > 0) ? 0.75 * envVal + 0.25 * postVal: envVal;

	
	if(isActive){
		
		for(int i = 0; i < 4; i++){
			offset[i] += (ofRandom(0,1) > 0.5)? 0.1: -0.1;
			offset[i] = max(1.0f, offset[i]);
			offset[i] = min(1.9f, offset[i]);
		}
		
		height = max(height * (1-swing), height * 0.05f);
		height += ofRandom(-swing * 5.0f, swing * 5.0f);
		width += ofRandom(-swing * 10.0f, swing * 10.0f);
		
	}
	
	b1.set(p1.x + width * offset[0], p1.y - height);
	b2.set(p2.x - width * offset[1], p2.y - height);
	bl1.set(p1.x + width * offset[2], p1.y + height); //concave version ... non symmetry// convex version uses 0110
	bl2.set(p2.x - width * offset[3], p2.y + height);
	
	setWrapData(ofVec2f(width, height), angle);

}


void belch::draw(int t_wrap){

	glPushMatrix();
	
	if(t_wrap == 0){
		glTranslatef(centre.x, centre.y, 0);
	}else if(t_wrap == 1){
		glTranslatef(wrapCoords.x, centre.y, 0); 
	}else if(t_wrap == 2){
		glTranslatef(centre.x, wrapCoords.y, 0); 
	}
	glRotatef(angle, 0, 0, 1);
	
	ofFill();
	ofSetColor(255);
	ofRect(blankRect);
	
	ofNoFill();
	ofSetColor(0);
	ofEnableSmoothing();
	ofBezier(
			 p1.x, p1.y, 
			 b1.x, b1.y,
			 b2.x ,b2.y,
			 p2.x, p2.y
			 
			 );

	ofBezier(
			 p1.x, p1.y, 
			 bl1.x, bl1.y,
			 bl2.x ,bl2.y,
			 p2.x, p2.y
			 
			 );
	
	ofDisableSmoothing();
	
	glPopMatrix();


}

float belch::getOffset(int t){return offset[t];}


vector<paramAttributes> belch::getParamDefs(){
	
	vector<paramAttributes> def;
	
	paramAttributes height;
	height.name = "height"; height.min_val = 20; height.max_val = 300; height.abs_value = 50;
	
	def.push_back(height);
	
	return def;
	
}