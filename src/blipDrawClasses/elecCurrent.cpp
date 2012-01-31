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
	waveHeight = max(1.0f, waveHeight);
	
	density = params[1];
	
	blankRect.setFromCenter(0, 0, length, 18);
	
	int numPeaks = (float)length * density;
	numPeaks = max(1, numPeaks);
	peaks.clear();
	
	peaks.push_back(ofVec2f(-length/2, 0));
	
	float pa = -length/2;
	float pb = pa + length/numPeaks;
	
	for(int i = 0; i < numPeaks; i ++){
		peaks.push_back(ofVec2f(ofRandom(pa,pb), ofRandom(-waveHeight,waveHeight)));
		pa = pb;
		pb = pa + (float)length/numPeaks;
	}

	peaks.push_back(ofVec2f(length/2, 0));
	setWrapData(ofVec2f(length/2,max_waveHeight),angle);
	
}

void elecCurrent::draw(int t_wrap){
	
	
	glPushMatrix();
	
		if(t_wrap == 0){
			glTranslatef(centre.x, centre.y, 0);
		}else if(t_wrap == 1){
			glTranslatef(wrapCoords.x, centre.y, 0); 
		}else if(t_wrap == 2){
			glTranslatef(centre.x, wrapCoords.y, 0); 
		}
		glRotatef(angle, 0, 0, 1);
		
	
		ofSetColor(255);
		ofFill();
		ofRect(blankRect);
	
		ofSetColor(100);
		ofLine(blankRect.x, -max_waveHeight, blankRect.x, max_waveHeight);
		ofLine(blankRect.x + blankRect.width, -max_waveHeight, blankRect.x + blankRect.width, max_waveHeight);

	
		if(isActive){
			
			ofNoFill();
			ofSetColor(0);
				
			ofBeginShape();
				
			for(int i = 0; i < peaks.size(); i ++)ofVertex(peaks[i].x, peaks[i].y);
		
			ofEndShape(false);
		
		}
	

	glPopMatrix();
	
	
	
}


vector<paramAttributes> elecCurrent::getParamDefs(){
	
	vector<paramAttributes> def;
	
	paramAttributes height, density;
	density.name = "density"; density.min_val = 0; density.max_val = 1; density.abs_value = 0.5;
	height.name = "height"; height.min_val = 10; height.max_val = 100; height.abs_value = 20;
	
	def.push_back(height);
	def.push_back(density);
	
	return def;
	
}