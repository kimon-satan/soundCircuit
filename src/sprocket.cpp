//
//  sprocket.cpp
//  soundcircuit
//
//  Created by Simon Katan on 20/05/2013.
//
//

#include "sprocket.h"


sprocket::sprocket(){

    pDefs = getParamDefs();
    c_rot = 0;
}


void sprocket::update(){

    alpha = 255.0f * envVal;
	c.setHsb(100,255,255, alpha);
	testRect.setFromCenter(0, 0, length, length);
    
	vector<ofVec2f> points;
	points.push_back(ofVec2f(testRect.x, testRect.y));
	points.push_back(ofVec2f(testRect.x + testRect.width, testRect.y + testRect.height));
	
    setBoundingRect(points, centre, angle);
    
	float swing = (postVal > 0) ? decayRatio * envVal + (1-decayRatio) * postVal: envVal;
    
    state = getParam("state");
    
    if(isActive){
    
       if(!isRot){
            c_rot = -90 *  (1 - envVal);
            if(envVal >= 0.99){
                c_rot = 0;
                isRot = true;
            }
        }
        
    }else{
        
        c_rot = 0;
        isRot = false;
    
    }
    

}


void sprocket::draw(int t_wrap){

    
	ofPushMatrix();
	
    ofSetRectMode(OF_RECTMODE_CENTER);
     ofEnableSmoothing();
	ofTranslate(centre.x, centre.y, 0);
	ofRotate(angle, 0, 0, 1);
    
    ofRotate(state * 90 + c_rot, 0,0,1);
	
    
    ofSetColor(255);
    ofFill();
    drawSprocket(testRect.width, testRect.height, length/4);
    ofTranslate(0,0,-2);
    
	if(isActive){
        
		ofFill();
		ofEnableAlphaBlending();
		ofSetColor(c);
		drawSprocket(testRect.width, testRect.height, length/4);
		ofDisableAlphaBlending();
		
	}
	
    
	ofSetColor(0);
	ofNoFill();
   
    glDepthFunc(GL_LEQUAL); //prevents visible segments
	drawSprocket(testRect.width, testRect.height, length/4);
    glDepthFunc(GL_LESS);
    
	ofPopMatrix();
    
    ofDisableSmoothing();
    ofSetRectMode(OF_RECTMODE_CORNER);

}


void sprocket::drawSprocket(float w, float h, float r){

    float nh = r; // indent depth
    float r2 = r; // top corner
    float r3 = r/2; //inner corner
    
    ofBeginShape();

    //br
    ofVertex(  w/2 , h/2 - r );
    ofBezierVertex( w/2 , h/2 - r , w/2, h/2,  w/2 - r, h/2);
    //bl
    ofVertex(  -w/2 + r, h/2 );
    ofBezierVertex( -w/2 + r , h/2, -w/2, h/2, -w/2 , h/2 - r);
    //tl
    ofVertex(  -w/2 , -h/2  + r);
    
    ofBezierVertex( -w/2 , -h/2 + r , -w/2, -h/2, -w/2 + r2/2, -h/2);
    
    //the dent
    ofBezierVertex( -w/2 + r2/2, -h/2 ,
                   -w/2 + r2 , -h/2 ,
                   -w/2 + r2 , -h/2 + nh/2);
    
    ofBezierVertex( -w/2 + r2, -h/2 + nh/2,
                   -w/2 + r2, -h/2 + nh,
                   -w/2 + r2 + r3, -h/2 + nh);
   
    ofVertex( -w/2 + r2 + r3 , -h/2 + nh);
    ofVertex(  w/2 - r2 - r3, -h/2 + nh);
    
    ofBezierVertex(  w/2 - r2 - r3, -h/2 + nh ,
                   w/2 - r2 , -h/2 + nh,
                   w/2 - r2, -h/2 + nh/2);
    
    ofBezierVertex(  w/2 - r2, -h/2 + nh/2,
                   w/2 - r2 , -h/2 ,
                   w/2 - r2/2, -h/2);
    
    ofBezierVertex( w/2 - r2/2, -h/2, w/2, -h/2,  w/2 , -h/2 + r);
    
    
    ofEndShape(true);

}


vector<paramAttributes> sprocket::getParamDefs(){
	
	vector<paramAttributes> def;
	
	paramAttributes t_att;
    
    t_att.name = "trackOffset"; t_att.min_val = -1;  t_att.max_val = 1;  t_att.abs_value = 0;
    def.push_back(t_att);
    t_att.name = "state"; t_att.min_val = 0;  t_att.max_val = 3;  t_att.abs_value = 0; t_att.modFrom = 0; t_att.modTo = 3;
    def.push_back(t_att);
    t_att.reset();

    
	
	return def;
	
}
