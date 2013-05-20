//
//  slider.cpp
//  soundcircuit
//
//  Created by Simon Katan on 13/05/2013.
//
//

#include "slider.h"
slider::slider():baseBlipDraw(){

    pDefs = getParamDefs();
	
}


void slider::update(){
	
 	testRect.setFromCenter(0, getParam("height")/2 * getParam("trackOffset"), length,getParam("height"));
	vector<ofVec2f> points;
	points.push_back(ofVec2f(testRect.x, testRect.y));
	points.push_back(ofVec2f(testRect.x + testRect.width, testRect.y + testRect.height));

	setBoundingRect(points, centre, angle);
	
	float swing = (postVal > 0) ? decayRatio * envVal + (1-decayRatio) * postVal: envVal;
    
    

	if(isActive){
        
        slide = getParam("height") * (1 - pow(swing,2));
        
	}else{
        
        slide = getParam("height");
		isDecay = false;

	}
	
	
}

void slider::draw(int t_wrap){
	

    
	ofPushMatrix();
    
	ofTranslate(centre.x, centre.y);
	ofRotate(angle, 0, 0, 1);
    if(getParam("orient") < 1)ofScale(1, -1);
    ofTranslate(0, -getParam("trackOffset") * getParam("height")/2);

    ofSetColor(0);
    ofNoFill();
    ofLine(0,0,0,getParam("height"));
    ofCircle(0, slide, getParam("slSize"));
    
	if(isActive){
        
    
		
		
	}

	
	ofPopMatrix();
	
	
}


vector<paramAttributes> slider::getParamDefs(){
	
    vector<paramAttributes> def;
	
	paramAttributes t_att;
    

	t_att.name = "height"; t_att.min_val = 30 * WORLD_UNIT; t_att.max_val = 300 * WORLD_UNIT; t_att.abs_value = 200 * WORLD_UNIT;
    def.push_back(t_att);
    t_att.name = "thickness"; t_att.min_val = 1; t_att.max_val = 5; t_att.abs_value = 1;
    def.push_back(t_att);
	t_att.name = "hue"; t_att.min_val = 15; t_att.max_val = 240; t_att.abs_value = 100;
    def.push_back(t_att);
    t_att.name = "sat"; t_att.min_val = 1; t_att.max_val = 255; t_att.abs_value = 100;
    def.push_back(t_att);
    t_att.name = "bri"; t_att.min_val = 1; t_att.max_val = 255; t_att.abs_value = 100;
    def.push_back(t_att);
    t_att.name = "trackOffset"; t_att.min_val = -1;  t_att.max_val = 1;  t_att.abs_value = 0;
    def.push_back(t_att);
    t_att.name = "orient"; t_att.min_val = 0;  t_att.max_val = 1;  t_att.abs_value = 1;
    def.push_back(t_att);
    t_att.name = "slSize"; t_att.min_val = 1;  t_att.max_val = 20;  t_att.abs_value = 5;
    def.push_back(t_att);

	
	return def;
	
}