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

elecCurrent::elecCurrent(){
	

}

void elecCurrent::update(){

	//0 = wave height
	//1 = density
	
	ofVec2f l_dir(length, length);
	l_dir *= direction;
	centre = startPos + l_dir/2;
	wrapCentre = endPos - l_dir/2;
	angle = (direction.y > 0) ? 90 : 0;
	max_waveHeight = params[0];
	waveHeight = params[0] * envVal;
	waveHeight = max(1.0f, waveHeight);
	density = params[1];
	
	blankRect.setFromCenter(0, 0, length, 18);
	
	int numPeaks = (float)length * density;
	numPeaks = max(1, numPeaks);
	peaks.clear();
	
	peaks.push_back(ofVec2f(-length/2, 0));
	
	
	float a = -length/2;
	float b = a + length/numPeaks;
	
	for(int i = 0; i < numPeaks; i ++){
		peaks.push_back(ofVec2f(ofRandom(a,b), ofRandom(-waveHeight,waveHeight)));
		a = b;
		b = a + (float)length/numPeaks;
	}

	peaks.push_back(ofVec2f(length/2, 0));

}

void elecCurrent::draw(bool isWrapped){
	
	glPushMatrix();
	
		if(isWrapped){
			glTranslatef(wrapCentre.x, wrapCentre.y, 0);
		}else{
			glTranslatef(centre.x, centre.y, 0); 
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