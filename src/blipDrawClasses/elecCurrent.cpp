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

void elecCurrent::draw(bool isWrapped){
	
	//0 = wave height
	//1 = density
	
	ofVec2f l_dir(length, length);
	l_dir *= direction;
	ofVec2f centre = (isWrapped) ? endPos - l_dir/2: startPos + l_dir/2;
	
	float angle = (direction.y > 0) ? 90 : 0;
	float waveHeight = params[0] * 100 * envVal;
	waveHeight = max(5.0f, waveHeight);
	float density = params[1];
	
	ofRectangle blankRect;
	blankRect.setFromCenter(0, 0, length, 18);
	
	glPushMatrix();
	
		glTranslatef(centre.x, centre.y, 0);
		glRotatef(angle, 0, 0, 1);

		glPushMatrix();
			glTranslatef(0, 0, 1);
			ofSetColor(100);
			ofLine(blankRect.x, blankRect.y, blankRect.x, blankRect.y + blankRect.height);
			ofLine(blankRect.x + blankRect.width, blankRect.y, blankRect.x + blankRect.width, blankRect.y + blankRect.height);
		glPopMatrix();
	
		if(isActive){
			
			ofSetColor(255);
			ofFill();
			ofRect(blankRect);
			
			glPushMatrix();
				ofNoFill();
				glTranslatef(0, 0, 1);
				ofSetColor(0);
				
				ofBeginShape();
				ofVertex(-length/2, 0);
				
				int numPeaks = (float)length * density;
				numPeaks = max(1, numPeaks);
			
				float a = -length/2;
				float b = a + length/numPeaks;
			
				for(int i = 0; i < numPeaks; i ++){
					ofVertex(ofRandom(a,b), ofRandom(-waveHeight,waveHeight));
					a = b;
					b = a + (float)length/numPeaks;
				}
				ofVertex(length/2, 0);
				ofEndShape(false);
			glPopMatrix();
		}
	

	glPopMatrix();
	
}