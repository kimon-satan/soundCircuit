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
	
	n_verts.clear();
	cps_a.clear();
	cps_b.clear();
	height = 0;
	width = 0;
	
}


void bean1::setup(ofVec2f t_dims){
	
	baseBlipDraw::setup(t_dims);
	
	if(n_verts.size() == 0){
		int numV = params[0];
		int midPoint = numV/2;
		
		for(int i = 0; i < numV; i ++){
			ofVec2f t_vert(ofRandom(params[2] -1 ,-1),0);
			t_vert.rotate((float)i  * (float)360/numV + (float)360/numV  * ofRandom(-params[3],params[3]), ofVec2f(0,0));
			
			if(i == 0)t_vert.set(-1,0);
			if(i == midPoint)t_vert.set(1,0);
			
			n_verts.push_back(t_vert);
			
			cps_a.push_back(ofVec2f(0,0));
			cps_b.push_back(ofVec2f(0,0));
		}
		
	}
	
	envVal = 0;
	postVal = 0;
}

void bean1::setup(ofVec2f t_dims, baseBlipDraw * t_draw){
	
	baseBlipDraw::setup(t_dims, t_draw);
	bean1 * b = (bean1 *)t_draw;
	cps_a = b->getCps_a();
	cps_b = b->getCps_b();
	n_verts = b->getN_verts();
	
	width = length/2;
	height = params[1];
	
}

void bean1::update(){
	
	width = length/2;
	height = params[1];
	
	blankRect.setFromCenter(0, 0, length -2, 2);
	
	float swing = (postVal > 0) ? decayRatio * envVal + (1-decayRatio) * postVal: envVal;
	
	vertices.clear();
	
	for(int i = 0; i < n_verts.size(); i++)vertices.push_back(ofVec2f(n_verts[i].x * width,n_verts[i].y * height));
	
	cps_a.clear();
	cps_b.clear();
	
	for(int i = 0; i < vertices.size(); i++){
		
		int n_i = (i+1)%vertices.size();
		
		if(i != 0 && i != vertices.size()/2)vertices[i].y *= 1 + (swing * params[8]);
		if(i != 0 && i != vertices.size()/2)vertices[i] *= 1 + (swing * params[9]);
		
		ofVec2f ta(vertices[i].getMiddle(vertices[n_i]));
		ofVec2f add_a( -ta * (params[4] + params[6] * swing));
		cps_a.push_back(ofVec2f(ta + add_a));
		
		ofVec2f tb_vec(cps_a.back() - vertices[i]);
		ofVec2f add_b( -tb_vec * (params[5] + params[7] * swing));
		cps_b.push_back(ofVec2f(vertices[i] - tb_vec + add_b));
		
		
	}
	
	setWrapData(ofVec2f(width, height), angle);
	
}


void bean1::draw(int t_wrap){
	
	//figure out craziness on preview draw
	
	glPushMatrix();
	
	if(t_wrap == 0){
		glTranslatef(centre.x, centre.y, 0);
	}else if(t_wrap == 1){
		glTranslatef(wrapCoords.x, centre.y, 0); 
	}else if(t_wrap == 2){
		glTranslatef(centre.x, wrapCoords.y, 0); 
	}
	glRotatef(angle, 0, 0, 1);
	
	if(params[11] > 0){
		ofFill();
		ofSetColor(255);
		ofRect(blankRect);
	}
	
	glTranslatef(0, params[1] * params[10], 0);
	
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


vector<paramAttributes> bean1::getParamDefs(){
	
	vector<paramAttributes> def;
	
	paramAttributes numV, height, displace_l, displace_r, a_add, b_add, a_swell, b_swell, v_swell, o_swell, trackOffset, blankTrack;
	
	//numv p0
	numV.name = "numV"; numV.min_val = 3; numV.max_val = 12; numV.abs_value = 6; //p0
	def.push_back(numV);
	
	height.name = "height"; height.min_val = 10; height.max_val = 100; height.abs_value = 20; //p1
	def.push_back(height);
	
	//displace_l p2
	displace_l.name = "displace_l"; displace_l.min_val = 0; displace_l.max_val = 1; displace_l.abs_value = 0.25; 
	def.push_back(displace_l);
	
	//displace_r p3
	displace_r.name = "displace_r"; displace_r.min_val = 0; displace_r.max_val = 1; displace_r.abs_value = 0.25; 
	def.push_back(displace_r);
	
	a_add.name = "a_add"; a_add.min_val = -1; a_add.max_val = 1; a_add.abs_value = 0; //4
	def.push_back(a_add);
	b_add.name = "b_add"; b_add.min_val = -1; b_add.max_val = 1; b_add.abs_value = 0; //5
	def.push_back(b_add);
	a_swell.name = "a_swell"; a_swell.min_val = -2; a_swell.max_val = 2; a_swell.abs_value = 0; //6
	def.push_back(a_swell);
	b_swell.name = "b_swell"; b_swell.min_val = -2; b_swell.max_val = 2; b_swell.abs_value = 0; //7
	def.push_back(b_swell);
	v_swell.name = "v_swell"; v_swell.min_val = 0; v_swell.max_val = 2; v_swell.abs_value = 0; //8
	def.push_back(v_swell);
	o_swell.name = "o_swell"; o_swell.min_val = 0; o_swell.max_val = 2; o_swell.abs_value = 0; //9
	def.push_back(o_swell);
	trackOffset.name = "trackOffset"; trackOffset.min_val = -1; trackOffset.max_val = 1; trackOffset.abs_value = 0; //10
	def.push_back(trackOffset);
	blankTrack.name = "blankTrack"; blankTrack.min_val = 0; blankTrack.max_val = 1; blankTrack.abs_value = 1; //11
	def.push_back(blankTrack);
	
	return def;
	
}