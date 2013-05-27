//
//  ratchet.cpp
//  soundcircuit
//
//  Created by Simon Katan on 27/05/2013.
//
//

#include "ratchet.h"
//
//  ratchet.cpp
//  soundcircuit
//
//  Created by Simon Katan on 13/05/2013.
//
//

#include "ratchet.h"
ratchet::ratchet():baseBlipDraw(){
    
    pDefs = getParamDefs();
    slide = 0;
    c_pos = 0;
    sl_mul = -1;
    alpha = 0;
    isDecay = false;
	
}


void ratchet::update(){
	
 	testRect.setFromCenter(0, getParam("height")/2 * getParam("trackOffset"), length,getParam("height"));
	vector<ofVec2f> points;
	points.push_back(ofVec2f(testRect.x, testRect.y));
	points.push_back(ofVec2f(testRect.x + testRect.width, testRect.y + testRect.height));
    
	setBoundingRect(points, centre, angle);

    
    float scope = getParam("slSize") * (1- getParam("height"));
    
	if(isActive){
        
        if(!isDecay && envVal > 0.99)isDecay = true;
        
        if(isDecay){
            slide = sl_mul/3.0 * scope * (1.0-envVal);
        }
        
        alpha = (envVal > 0.05)? 255:0;
        
        
	}else{
        
        
        if(abs(slide) > 0){
            cout << scope << "," << c_pos << endl;
            c_pos += slide;
            if(abs(c_pos) - abs(scope) <= 5 || abs(c_pos) <= 5){
               sl_mul *=-1;
            }
            slide = 0;
        }
        
		isDecay = false;
        alpha = 0;
        
	}
	
	
}

void ratchet::draw(int t_wrap){
	
    
    
	ofPushMatrix();
    
	ofTranslate(centre.x, centre.y);
	ofRotate(angle, 0, 0, 1);
    if(getParam("orient") < 1)ofScale(1, -1);
    ofTranslate(0, -getParam("trackOffset") * getParam("slSize")/2);
    
    ofSetColor(0);
    ofNoFill();
    
    ofPushMatrix();
        ofTranslate(0, c_pos);
        ofTranslate(0, slide);
        ofTranslate(0, -getParam("height") * getParam("slSize"));
        ofLine(0,0,0,getParam("slSize"));
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(0,0,-2);

        
    if(isActive){
        ofFill();
        ofEnableAlphaBlending();
        ofSetColor(0, 255, 0, alpha);
        ofCircle(0, -getParam("height") * getParam("slSize"), 5);
        ofDisableAlphaBlending();

    }

    ofSetColor(0);
    ofNoFill();
    ofEnableSmoothing();
    glDepthFunc(GL_LEQUAL);
    ofCircle(0, -getParam("height") * getParam("slSize"), 5);
    glDepthFunc(GL_LESS);
    ofDisableSmoothing();
    
    ofPopMatrix();
    
	
	ofPopMatrix();
	
	
}


vector<paramAttributes> ratchet::getParamDefs(){
	
    vector<paramAttributes> def;
	
	paramAttributes t_att;
    
    
	t_att.name = "height"; t_att.min_val = t_att.min_val = 0.25;  t_att.max_val = 0.75;  t_att.abs_value = 0;
    def.push_back(t_att);
    t_att.name = "sus"; t_att.min_val = 0; t_att.max_val = 1; t_att.abs_value = 1;
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
    t_att.name = "slSize"; t_att.min_val = 50;  t_att.max_val = 300 * WORLD_UNIT;  t_att.abs_value = 200 * WORLD_UNIT;
    def.push_back(t_att);
    
	
	return def;
	
}