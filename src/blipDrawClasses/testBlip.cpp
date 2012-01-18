/*
 *  testBlip.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 28/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "testBlip.h"
#include "blipPreset.h"
#include "blip.h"


void testBlip::update(){

	ofVec2f l_dir(length, length);
	l_dir *= direction;
	
	centre = startPos + l_dir/2;
	wrapCentre = endPos - l_dir/2;
	angle = (direction.y > 0) ? 90 : 0;
	alpha = 255.0f * envVal;
	c.setHsb(params[0] * 255,255,255, alpha);
	testRect.setFromCenter(0, 0, length, 18);

}

void testBlip::draw(bool isWrapped){
	
	
	glPushMatrix();
	
	if(isWrapped){
		glTranslatef(wrapCentre.x, wrapCentre.y, 0);
	}else{
		glTranslatef(centre.x, centre.y, 0); 
	}
	
	glRotatef(angle, 0, 0, 1);
	
	if(isActive){

		ofFill();
		ofEnableAlphaBlending();
		ofSetColor(c);
		ofRect(testRect);
		ofDisableAlphaBlending();
		
	}
	

	ofSetColor(0);
	ofNoFill();
	ofRect(testRect);

	
	glPopMatrix();
	

}
