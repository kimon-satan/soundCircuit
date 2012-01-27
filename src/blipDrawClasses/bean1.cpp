/*
 *  bean1.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 26/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "bean1.h"

/*
 *  belch.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 24/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "bean1.h"

bean1::bean1():baseBlipDraw(){
	
	
	
}


void bean1::setup(ofVec2f t_dims){
	
	baseBlipDraw::setup(t_dims);
	
	if(vertices.size() == 0){
		int numV = 5;
		int midPoint = numV/2;
		
		for(int i = 0; i < numV; i ++){
			ofVec2f t_vert(ofVec2f(0,0) + ofVec2f(ofRandom(-0.5,-1),0));
			t_vert.rotate((float)i  * (float)360/numV + (float)360/numV  * ofRandom(-0.5,0.5), ofVec2f(0,0));
			
			if(i == 0)t_vert.set(-1,0);
			if(i == midPoint)t_vert.set(1,0);
			
			n_verts.push_back(t_vert);
			vertices.push_back(t_vert * length/2);
			cps_a.push_back(ofVec2f(0,0));
			cps_b.push_back(ofVec2f(0,0));
		}
		
	}else{
	
		for(int i = 0; i < vertices.size(); i ++)vertices[i] = n_verts[i] * length/2; 
	}
	
	width = length/2;
	height = length/2;
	

}

void bean1::setup(ofVec2f t_dims, baseBlipDraw * t_draw){
	
	baseBlipDraw::setup(t_dims, t_draw);
	bean1 * b = (bean1 *)t_draw;
	vertices = b->getVertices();
	cps_a = b->getCps_a();
	cps_b = b->getCps_b();
	n_verts = b->getN_verts();
	
	width = length/2;
	height = length/2;
	
}

void bean1::update(){
	
	blankRect.setFromCenter(0, 0, length -2, 2);
	
	bool concave = false;
	
	float swing = (postVal > 0) ? 0.3 * envVal + 0.7 * postVal: envVal;
	//if(isActive)cout << swing << "\n";
	
	for(int i = 0; i < vertices.size(); i++){
		
		float d =(isActive)? 6.0f * (1.0f - swing): 6; // the smoothness
		d = max(1.0f, d);
		
		int n_i = (i + 1)%vertices.size();
		
		ofVec2f t_vec(vertices[n_i] - vertices[i]);
		ofVec2f add(-t_vec.x/d,t_vec.y/d);
		
		if(t_vec.y > 0 && t_vec.x > 0){
			add *= -1;
		}else if(t_vec.x < 0  && t_vec.y < 0){
			add *= - 1;
		}
		
		if(concave)add*= -1;
		
		cps_a[i].set(ofVec2f (vertices[i].getMiddle(vertices[n_i]) + add));
		
		ofVec2f tc_vec(cps_a[i] - vertices[i]);
		cps_b[i].set(vertices[i] - tc_vec);
		
		concave = !concave;
	}
	
	if(isActive){
		

		
	}
	
	setWrapData(ofVec2f(width, height), angle);
	
}


void bean1::draw(int t_wrap){
	
	glPushMatrix();
	
	if(t_wrap == 0){
		glTranslatef(centre.x, centre.y, 0);
	}else if(t_wrap == 1){
		glTranslatef(wrapCoords.x, centre.y, 0); 
	}else if(t_wrap == 2){
		glTranslatef(centre.x, wrapCoords.y, 0); 
	}
	glRotatef(angle, 0, 0, 1);
	
	ofFill();
	ofSetColor(255);
	ofRect(blankRect);
	
	ofNoFill();
	ofSetColor(0);
	
	ofEnableSmoothing();
	
	ofBeginShape();
	ofVertex(vertices[0].x, vertices[0].y);
	for(int i = 0; i < vertices.size(); i ++){
		int n_i = (i + 1)%vertices.size();
		ofBezierVertex(cps_a[i].x, cps_a[i].y, 
					   cps_b[n_i].x,cps_b[n_i].y, 
					   vertices[n_i].x,vertices[n_i].y);
	}
	ofVertex(vertices[0].x, vertices[0].y);
	ofEndShape(true);
	
	ofDisableSmoothing();
	
	glPopMatrix();
	
}


vector<ofVec2f>bean1::getVertices(){ return vertices;}
vector<ofVec2f>bean1::getN_verts(){return n_verts;}
vector<ofVec2f>bean1::getCps_a(){ return cps_a; }
vector<ofVec2f>bean1::getCps_b(){ return cps_b; }