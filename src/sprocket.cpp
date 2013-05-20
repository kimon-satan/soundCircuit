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

}


void sprocket::update(){

    alpha = 255.0f * envVal;
	c.setHsb(100,255,255, alpha);
	testRect.setFromCenter(0, 0, length, length);
    

}


void sprocket::draw(int t_wrap){

    
	glPushMatrix();
	
    ofSetRectMode(OF_RECTMODE_CENTER);
     ofEnableSmoothing();
	glTranslatef(centre.x, centre.y, 2);
	glRotatef(angle, 0, 0, 1);
	
    
    
	if(isActive){
        
		ofFill();
		ofEnableAlphaBlending();
		ofSetColor(c);
       
		ofRect(0,0, testRect.width, testRect.height);
		ofDisableAlphaBlending();
		
	}
	
    
	ofSetColor(0);
	ofNoFill();
   
    glDepthFunc(GL_LEQUAL); //prevents visible segments
	drawSprocket(testRect.width, testRect.height, length/6);
    glDepthFunc(GL_LESS);
    
	glPopMatrix();
    
    ofDisableSmoothing();
    ofSetRectMode(OF_RECTMODE_CORNER);

}


void sprocket::drawSprocket(float w, float h, float r){

    ofBeginShape();
    ofVertex(  w/2 - r, -h/2 );
    ofBezierVertex( w/2 - r , -h/2, w/2, -h/2, w/2 , -h/2 + r);
    ofVertex(  w/2 , h/2 - r );
    ofBezierVertex( w/2 , h/2 - r , w/2, h/2,  w/2 - r, h/2);
    ofVertex(  -w/2 + r, h/2 );
    ofBezierVertex( -w/2 + r , h/2, -w/2, h/2, -w/2 , h/2 - r);
    ofVertex(  -w/2 , -h/2  +r );
    ofBezierVertex( -w/2 , -h/2 + r , -w/2, -h/2,  -w/2 + r, -h/2);
    ofVertex(  -w/2 + r , -h/2 );
    ofVertex(  -w/2 + r , -h/2 - r);
    ofVertex(  w/2 - r , -h/2 - r);
    ofVertex(  w/2 - r, -h/2 );
    ofEndShape(false);

}


vector<paramAttributes> sprocket::getParamDefs(){
	
	vector<paramAttributes> def;
	
	paramAttributes t_att;
    
    t_att.name = "trackOffset"; t_att.min_val = -1;  t_att.max_val = 1;  t_att.abs_value = 0;
    def.push_back(t_att);
    
    
	
	return def;
	
}
