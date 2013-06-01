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
    wobble = 0;
	
}


void slider::update(){
	
 	testRect.setFromCenter(0, -getParam("slSize")/2 , getParam("headSize"), getParam("slSize"));
	vector<ofVec2f> points;
	points.push_back(ofVec2f(testRect.x, testRect.y));
	points.push_back(ofVec2f(testRect.x + testRect.width, testRect.y + testRect.height));

	setBoundingRect(points, centre, angle);
	
	
	if(isActive){
        
        if(envVal > 0.99)isDecay = true;
        
        if(getParam("sus") == 0){
            slide =  (postVal > 0) ? decayRatio * envVal + (1-decayRatio) * postVal: envVal;
        }else{
            if(!isDecay){
                slide = envVal;
            }else{
                if(envVal < 1- getParam("sus")){
                    slide = envVal * (1.0/(1.0 - getParam("sus")));
                    
                }else{
                    slide = 1;
                }
            }
        
        }
        
        slide = pow(slide,2);
        wobble = slide * getParam("wobble");
        slide = 1 - slide * getParam("range");
            
	}else{
        
        slide = 1;
        isDecay = false;

	}
    
    
    c_pos = -slide  * getParam("slSize");
    
	if(wobble > 0){
        wobblePos.set(0,ofRandom(-wobble,wobble));
    }else{
        wobblePos.set(0,0);
    }
	
}

void slider::draw(int t_wrap){
	

    
    ofPushMatrix();
    
	ofTranslate(centre.x, centre.y);
	ofRotate(angle, 0, 0, 1);
    if(getParam("orient") < 1)ofScale(1, -1);
    
    
    ofPushStyle();
        ofSetColor(getParam("linecol"));
        ofPushMatrix();
        ofTranslate(0, -getParam("slSize"));
        ofSetLineWidth(getParam("thickness"));
        ofLine(0,0,0,getParam("slSize"));
        ofPopMatrix();
    ofPopStyle();
    
    
    ofPushMatrix();
    
        ofTranslate(wobblePos.x, wobblePos.y, -2);
        glDepthFunc(GL_LEQUAL);
        ofFill();
        ofSetColor(255);
        ofCircle(0, c_pos, getParam("headSize"));
        
        
        if(isActive){
            
            ofFill();
            ofEnableAlphaBlending();
            ofSetColor(c);
            ofCircle(0, c_pos, getParam("headSize"));
            ofDisableAlphaBlending();
            
        }
        
        
        ofSetColor(0);
        ofNoFill();
       // ofEnableSmoothing();
        ofCircle(0, c_pos, getParam("headSize"));
       // ofDisableSmoothing();
        glDepthFunc(GL_LESS);
    
    
	
        ofPopMatrix();
    ofPopMatrix();
	
	
}


vector<paramAttributes> slider::getParamDefs(){
	
    vector<paramAttributes> def;
	
	paramAttributes t_att;
    

	t_att.name = "range"; t_att.min_val = 0.25; t_att.max_val = 1.0; t_att.abs_value = 0.75;
    def.push_back(t_att);
    t_att.name = "sus"; t_att.min_val = 0; t_att.max_val = 1; t_att.abs_value = 0;
    def.push_back(t_att);
    t_att.name = "thickness"; t_att.min_val = 1; t_att.max_val = 5; t_att.abs_value = 1;
    def.push_back(t_att);
	t_att.name = "hue"; t_att.min_val = 15; t_att.max_val = 240; t_att.abs_value = 100;
    def.push_back(t_att);
    t_att.name = "orient"; t_att.min_val = 0;  t_att.max_val = 1;  t_att.abs_value = 1;
    def.push_back(t_att);
    t_att.name = "slSize"; t_att.min_val = 100;  t_att.max_val = 300;  t_att.abs_value = 150;
    def.push_back(t_att);
    t_att.name = "headSize"; t_att.min_val = 5;  t_att.max_val = 30;  t_att.abs_value = 5;
    def.push_back(t_att);
    t_att.name = "wobble"; t_att.min_val = 0;  t_att.max_val = 10;  t_att.abs_value = 3;
    def.push_back(t_att);
    t_att.name = "linecol"; t_att.min_val = 0;  t_att.max_val = 255;  t_att.abs_value = 100;
    def.push_back(t_att);

	
	return def;
	
}