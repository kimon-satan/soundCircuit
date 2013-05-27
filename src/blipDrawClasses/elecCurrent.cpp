/*
 *  elecCurrent.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 17/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "elecCurrent.h"
#include "blipPreset.h"
#include "blip.h"

elecCurrent::elecCurrent():baseBlipDraw(){
	

}

void elecCurrent::update(){

	//0 = wave height
	//1 = density
	
	max_waveHeight = params[0];
	float swing = (postVal > 0) ? 0.3 * envVal + 0.7 * postVal: envVal;
	waveHeight = params[0] * swing;
	waveHeight = max((float)WORLD_UNIT, waveHeight);
	
	density = params[1];
	
	blankRect.setFromCenter(0, 0, length, WORLD_UNIT * 10);
	
	int numPeaks = (float)length  * density;
	numPeaks = max(1, numPeaks);
	peaks.clear();
	
	peaks.push_back(ofVec2f(-length/2 + WORLD_UNIT * 2, 0));
	
	float pa = peaks[0].x;
	float pb = pa + (float)(length - WORLD_UNIT *2)/(numPeaks);
	
	for(int i = 0; i < numPeaks; i ++){
		peaks.push_back(ofVec2f(ofRandom(pa,pb), ofRandom(-waveHeight,waveHeight)));
		pa = pb;
		pb = pa + (float)(length - WORLD_UNIT *2)/(numPeaks);
	}

	peaks.push_back(ofVec2f(length/2 - WORLD_UNIT * 2, 0));
	setWrapData(ofVec2f(length/2,max_waveHeight),angle);

	
}

void elecCurrent::draw(int t_wrap){
	

	glPushMatrix();
	
		glTranslatef(centre.x, centre.y, 0);
		glRotatef(angle, 0, 0, 1);
		
	
		ofSetColor(255);
		ofFill();
		ofRect(blankRect);
		
		ofPushMatrix();
	
		ofTranslate(0, 0, -(float)WORLD_UNIT/2.0f);
		if(isActive){
			
			ofNoFill();
			ofSetColor(0);
				
			ofBeginShape();
				
			for(int i = 0; i < peaks.size(); i ++)ofVertex(peaks[i].x, peaks[i].y);
			ofEndShape(false);
		
		}
	
		ofTranslate(0, 0, -(float)WORLD_UNIT/2.0f);
	
		ofPushStyle();
		ofSetColor(100);
		ofSetLineWidth(params[2]); //replace lines with rectangles 
		ofLine(blankRect.x + params[2] * WORLD_UNIT/2, -max_waveHeight, blankRect.x + params[2] * WORLD_UNIT/2, max_waveHeight);
		ofSetLineWidth(params[3]);
		ofLine(blankRect.x + blankRect.width - params[3]* WORLD_UNIT/2, -max_waveHeight, blankRect.x + blankRect.width - params[3] *WORLD_UNIT/2, max_waveHeight);
		ofPopStyle();
	
		ofPopMatrix();
	

	glPopMatrix();
	
}


vector<paramAttributes> elecCurrent::getParamDefs(){
	
	vector<paramAttributes> def;
	
	paramAttributes height, density, thick_l, thick_r;
	
	density.name = "density"; density.min_val = 0; density.max_val = 1; density.abs_value = 0.5;
	height.name = "height"; height.min_val = 10 * WORLD_UNIT; height.max_val = 100 * WORLD_UNIT; height.abs_value = 20 * WORLD_UNIT;
	thick_l.name = "thick_l"; thick_l.min_val = 1; thick_l.max_val = 5; thick_l.abs_value = 1;
	thick_r.name = "thick_r"; thick_r.min_val = 1; thick_r.max_val = 5; thick_r.abs_value = 1;
	
	def.push_back(height);
	def.push_back(density);
	def.push_back(thick_l);
	def.push_back(thick_r);
	
	return def;
	
}