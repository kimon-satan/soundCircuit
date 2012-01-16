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

void testBlip::draw(bool isWrapped){
	
	//test change
	
	ofVec2f l_dir(length, length);
	l_dir *= direction;
	ofVec2f centre = (isWrapped) ? endPos - l_dir/2: startPos + l_dir/2;
	float angle = (direction.y > 0) ? 90 : 0;
	float alpha = 255.0f * envVal;
	ofColor c;
	c.setHsb(params[0] * 255,255,255, alpha);
	
	ofRectangle testRect;
	testRect.setFromCenter(0, 0, length, 18);
	
	glPushMatrix();
	
	glTranslatef(centre.x, centre.y, 0);
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
