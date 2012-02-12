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
	testRect.setFromCenter(0, 0, length, params[1]);

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


vector<paramAttributes> testBlip::getParamDefs(){

	vector<paramAttributes> def;
	
	paramAttributes hue, height;
	hue.name = "hue"; hue.min_val = 0; hue.max_val = 255; hue.abs_value = 100;
	height.name = "height"; height.min_val = 10; height.max_val = 100; height.abs_value = 20;
	
	def.push_back(hue);
	def.push_back(height);
	
	return def;

}