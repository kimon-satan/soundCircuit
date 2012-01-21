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
	waveHeight = params[0] * envVal;
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