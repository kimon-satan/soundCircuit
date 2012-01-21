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

testBlip::testBlip():baseBlipDraw(){
	
}


void testBlip::update(){	

	alpha = 255.0f * envVal;
	c.setHsb(params[0],255,255, alpha);
	testRect.setFromCenter(0, 0, length, 18);

	setWrapData(ofVec2f(length/2,9),angle);

}

void testBlip::draw(int t_wrap){
	
	
	glPushMatrix();
	
	if(t_wrap == 0){
		glTranslatef(centre.x, centre.y, 0);
	}else if(t_wrap == 1){
		glTranslatef(wrapCoords.x, centre.y, 0); 
	}else if(t_wrap == 2){
		glTranslatef(centre.x, wrapCoords.y, 0); 
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
