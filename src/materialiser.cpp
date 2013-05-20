//
//  materialiser.cpp
//  soundcircuit
//
//  Created by Simon Katan on 16/05/2013.
//
//

#include "materialiser.h"


materialiser::materialiser():baseBlipDraw(){
	
	pDefs = getParamDefs();
	
}


void materialiser::update(){
	
	testRect.setFromCenter(0, getParam("height")/2 * getParam("trackOffset"), length,getParam("height"));
	vector<ofVec2f> points;
	points.push_back(ofVec2f(testRect.x, testRect.y));
	points.push_back(ofVec2f(testRect.x + testRect.width, testRect.y + testRect.height));
	
	
	float swing = (postVal > 0) ? decayRatio * envVal + (1-decayRatio) * postVal: envVal;
    
	
	if(isActive){
		
		grow = swing;
        wobble.set(swing * ofRandom(-getParam("hWob"), getParam("hWob")),swing * ofRandom(-getParam("vWob"),getParam("vWob")));
        rock = ofRandom(-getParam("rock") * grow, getParam("rock") * grow);
        
        for(int i = 0; i < points.size(); i++){
            points[i] += wobble;
            points[i].rotate(rock, wobble + ofVec2f(0,getParam("height")/2 * getParam("trackOffset")));
            
        }
        
       
        
	}else{
		
		isDecay = false;
        wobble.set(0,0);
        rock = 0;
	}
    
     setBoundingRect(points, centre, angle);
	
	
}

void materialiser::draw(int t_wrap){
	
	ofPushMatrix();
	
	ofTranslate(centre.x, centre.y, 0);
    ofRotate(angle, 0, 0, 1);

	ofTranslate(0, getParam("height")/2 * getParam("trackOffset"), 0);
	
    float r = (length < getParam("height"))? length/2 : getParam("height")/2 - 2;
    
    ofTranslate(wobble.x, wobble.y, 0);
    ofRotate(rock, 0, 0, 1);
    
    ofFill();
    ofSetColor(255);
    drawRoundedRect(length - 2, getParam("height") - 2, r);
    
    ofTranslate(0, 0,-2);
    
    glDepthFunc(GL_LEQUAL); //prevents visible segments
    
    ofPushStyle();
    ofNoFill();
    ofSetColor(0);
    ofEnableSmoothing();
    ofSetLineWidth(getParam("thickness"));
    drawRoundedRect(length - 2, getParam("height") - 2, r);
    ofPopStyle();
    
    glDepthFunc(GL_LESS);
    
    if(isActive){

            ofFill();
            ofSetColor(0);
        
            int nps = max(1, (int)(getParam("numPs") * pow(grow,2)));
        
            for(int i =0; i < nps; i++){

                //calc coordinates
                
                float w = length - 2;
                float h = getParam("height");
                float r = getParam("height")/2;
                
                ofColor c;
                c.setHsb(getParam("hue"),ofRandom(255-getParam("satRng"), 255),ofRandom(255-getParam("briRng"), 255));
                ofSetColor(c);
                
                int ps = ofRandom(1,getParam("pSize"));
                ofCircle(genRectPoint(w - ps,h - ps, r - ps), ps);
               
            }
        
	}

	
	ofPopMatrix();
	
	
}


ofVec2f materialiser::genRectPoint(float w, float h, float r){
    
    float px = ofRandom(-1,1) * w/2;
    float py = ofRandom(-1,1) * h/2;
    
    if(abs(px) > w/2  - r && abs(py) > h/2 - r){
        
        float mx = abs(px) - w/2 + r;
        float mv_y = pow(r,2)- pow(mx,2);
        mv_y = max(0.0f,sqrt(mv_y));
        
        float my = ofRandom(0,1) * mv_y;
        
        if(py < 0){
            py = r - my - h/2;
        }else{
            py = my + h/2 - r;
        }
        
    }
    
    return ofVec2f(px,py);
    
}


void materialiser::drawRoundedRect(float w, float h , float r){

    ofBeginShape();
    ofVertex(  w/2 - r, -h/2 );
    ofBezierVertex( w/2 - r , -h/2, w/2, -h/2, w/2 , -h/2 + r);
    ofVertex(  w/2 , h/2 - r );
    ofBezierVertex( w/2 , h/2 - r , w/2, h/2,  w/2 - r, h/2);
    ofVertex(  -w/2 + r, h/2 );
    ofBezierVertex( -w/2 + r , h/2, -w/2, h/2, -w/2 , h/2 - r);
    ofVertex(  -w/2 , -h/2  +r );
    ofBezierVertex( -w/2 , -h/2 + r , -w/2, -h/2,  -w/2 + r, -h/2);
    ofEndShape(true);
    
}

vector<paramAttributes> materialiser::getParamDefs(){
	
	vector<paramAttributes> def;
	
	paramAttributes t_att;
    
    t_att.name = "rock"; t_att.min_val = 2 * WORLD_UNIT; t_att.max_val = 50 * WORLD_UNIT; t_att.abs_value = 10 * WORLD_UNIT;
    def.push_back(t_att);
    t_att.name = "vWob"; t_att.min_val = 2 * WORLD_UNIT; t_att.max_val = 50 * WORLD_UNIT; t_att.abs_value = 10 * WORLD_UNIT;
    def.push_back(t_att);
    t_att.name = "hWob";t_att.min_val = 2 * WORLD_UNIT; t_att.max_val = 50 * WORLD_UNIT; t_att.abs_value = 10 * WORLD_UNIT;
    def.push_back(t_att);
	t_att.name = "height"; t_att.min_val = 30 * WORLD_UNIT; t_att.max_val = 100 * WORLD_UNIT; t_att.abs_value = 20 * WORLD_UNIT;
    def.push_back(t_att);
    t_att.name = "thickness"; t_att.min_val = 1; t_att.max_val = 5; t_att.abs_value = 1;
    def.push_back(t_att);
	t_att.name = "hue"; t_att.min_val = 15; t_att.max_val = 240; t_att.abs_value = 100;
    def.push_back(t_att);
    t_att.name = "satRng"; t_att.min_val = 1; t_att.max_val = 255; t_att.abs_value = 100;
    def.push_back(t_att);
    t_att.name = "briRng"; t_att.min_val = 1; t_att.max_val = 255; t_att.abs_value = 100;
    def.push_back(t_att);
    t_att.name = "numPs";  t_att.min_val = 1;  t_att.max_val = 1000;  t_att.abs_value = 500;
    def.push_back(t_att);
    t_att.name = "pSize"; t_att.min_val = 1;  t_att.max_val = 10;  t_att.abs_value = 5;
    def.push_back(t_att);
    t_att.name = "trackOffset"; t_att.min_val = -1;  t_att.max_val = 1;  t_att.abs_value = 0;
    def.push_back(t_att);
    
	
	
  
	
	return def;
	
}



