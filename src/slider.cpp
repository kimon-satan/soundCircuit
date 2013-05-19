//
//  slider.cpp
//  soundcircuit
//
//  Created by Simon Katan on 13/05/2013.
//
//

#include "slider.h"
slider::slider():baseBlipDraw(){
	
    slide = 0;
	
}


void slider::update(){
	
    testRect.setFromCenter(0, 0, length, params[1]);
	//setWrapData(ofVec2f(length/2,params[0]),angle);
	vector<ofVec2f> points;
	points.push_back(ofVec2f(testRect.x, -params[0]/2));
	points.push_back(ofVec2f(testRect.x + testRect.width, testRect.y + params[0]/2));

	
	
	setBoundingRect(points, centre, angle);
	
	float swing = (postVal > 0) ? decayRatio * envVal + (1-decayRatio) * postVal: envVal;
    
	
	c.setHsb(params[2],255,255,255);
    

	if(isActive){
        
        strobe = (ofRandom(0,1) > 0.5);
        wobble = 10 * envVal;
        hor_wob = ofRandom(-wobble,wobble);
        ver_wob = ofRandom(-wobble,wobble);
        
	}else{
        
		isDecay = false;
        hor_wob = 0;
        ver_wob = 0;
        strobe = false;
	}
	
	
}

void slider::draw(int t_wrap){
	

    
	glPushMatrix();
    
	glTranslatef(centre.x, centre.y, 0);
	glRotatef(angle, 0, 0, 1);
	
	
	glTranslatef(0, params[1] * params[0]/2, 0);
    ofPushMatrix();
    glTranslatef(hor_wob, ver_wob, 0);
	
    ofSetRectMode(OF_RECTMODE_CENTER);
    
	if(isActive){
        
        ofFill();
		ofEnableAlphaBlending();
		ofSetColor(c);
		ofRect(0, 0, 0, length, params[0]);
		ofDisableAlphaBlending();
		
		
	}
    ofPopMatrix();
	
    ofSetColor(80);
    ofNoFill();
    ofRect(0, 0, 0,length,params[0]);
    
    ofNoFill();
    ofSetColor(0);
    ofRect(0, 0, 0,length/2,10);
	
	ofSetRectMode(OF_RECTMODE_CORNER);
	
	glPopMatrix();
	
	
}


vector<paramAttributes> slider::getParamDefs(){
	
	vector<paramAttributes> def;
	
	paramAttributes height, hue, trackOffset;
	height.name = "height"; height.min_val = 10 * WORLD_UNIT; height.max_val = 100 * WORLD_UNIT; height.abs_value = 20 * WORLD_UNIT;
    trackOffset.name = "trackOffset"; trackOffset.min_val = -1; trackOffset.max_val = 1; trackOffset.abs_value = 0;

	hue.name = "hue"; hue.min_val = 15; hue.max_val = 240; hue.abs_value = 100;
	
	def.push_back(height);
    def.push_back(trackOffset);
    
    def.push_back(hue); //2

	
	return def;
	
}