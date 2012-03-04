/*
 *  layer.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 04/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "layer.h"

layer::layer():objectRenderer(){

	isScreenData = false;
	mReader.setLayer(this);
	incr = mReader.getIncrement();

	for(int i =0; i < 3; i++){
		rots.push_back(0);
		targetRots.push_back(0);
		trans.push_back(0);
		targetTrans.push_back(0);
	}

}

layer::layer(const layer &src):objectRenderer(){
	
	mReader.setLayer(this);
	world_dims = src.getWorldDims();
	incr = mReader.getIncrement();
	isScreenData = false;
	
	blips = src.getBlips();
	nodes = src.getNodes();
	tracks = src.getTracks();
	
	mReader = src.getReader();
	mReader.setLayer(this);
	
	trans = src.getTrans();
	rots = src.getRots();
	targetTrans = src.getTargetTrans();
	targetRots = src.getTargetRots();
	
	isScreenData = false;

}

void layer::update(){

	mReader.update();
	for(vector<blip>::iterator it = blips.begin(); it != blips.end(); it++)it->update();
	
	if(abs(rots[2] - targetRots[2]) > 2){
		
		if(rots[2] < targetRots[2]){ 
			rots[2] += 1;
		}else{
			rots[2] -= 1;
		}
		
	}else{
		rots[2] = targetRots[2];
	}
}

void layer::draw(ofRectangle &vp, bool isDummy){
	
	glPushMatrix();
	glTranslatef(ofGetScreenWidth()/2, ofGetScreenHeight()/2, 0);
	glPushMatrix();
	glTranslatef(-vp.x, -vp.y,0); //centred coordinates
	
	//for wrapping debugging 
	if(isScreenData){
		glPushMatrix();
		//glTranslatef(0, 0, -3);
		ofEnableAlphaBlending();
		(isDummy)?ofSetColor(255,0,0,100) : ofSetColor(0, 255, 0, 100);
		ofFill();
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofRect(0,0,world_dims.x, world_dims.y);
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofDisableAlphaBlending();
		ofNoFill();
		ofSetColor(0);
		glPopMatrix();
	}
	
	//calculate area that will be viewed
	
	ofRectangle viewable;
	
	viewable.x = max(-world_dims.x/2 - 2,-vp.width/2 + vp.x);
	viewable.x = min(world_dims.x/2 + 2,viewable.x);
	viewable.y = max(-world_dims.y/2 - 2,-vp.height/2 + vp.y);
	viewable.y = min(world_dims.y/2 + 2,viewable.y);
	
	ofVec2f br;
	br.x = max(-world_dims.x/2 - 2,vp.width/2  +vp.x);
	br.x = min(world_dims.x/2 + 2,br.x);
	br.y = max(-world_dims.y/2 - 2,vp.height/2  +vp.y);
	br.y = min(world_dims.y/2 + 2,br.y);
	
	viewable.width  = abs(viewable.x - br.x) + 5;
	viewable.height = abs(viewable.y - br.y) + 5;
	
	render(viewable);
	
	mReader.draw(); // will need to draw in a separate method for drawing orders
	
	glPopMatrix();
	
	glPopMatrix();
}


void layer::selectSomething(ofVec2f w_pos){

	deselectTracks();
	deselectNodes();
	if(!selectNode(w_pos)){
		selectTrackPoint(w_pos);
	}

}

void layer::deselectAll(){

	deselectNodes();
	deselectTracks();
}


void layer::beginInsertion(ofVec2f t_pos, ofVec2f t_dir){

	mReader.beginInsertion(t_pos, t_dir);
	objectManager::beginInsertion(t_pos, t_dir);
	
}

void layer::resizeInsertion(float size){
	
	mReader.resizeInsertion(size);
	objectManager::resizeInsertion(size);

}

void layer::endInsertion(){
	
	objectManager::endInsertion();
	
}

void layer::rotate(int plane){
	
	switch (plane) {
		case 2:
			targetRots[2] = (targetRots[2] == 0)? -90 : 0;
			break;
	}
	
	
}

void layer::toggleScreenData(){isScreenData = !isScreenData;}

//getters and setters


vector<node> * layer::getNodesRef(){return &nodes;}
vector<blip> * layer::getBlipsRef(){return &blips;}

vector<node> layer::getNodes()const{return nodes;}
vector<blip> layer::getBlips()const{return blips;}
vector<segment> layer::getTracks()const{return tracks;}

reader * layer::getReaderRef(){return &mReader;}
reader layer::getReader()const{return mReader;}

vector<float> layer::getTrans()const{return trans;}
vector<float> layer::getRots()const{return rots;}
vector<float> layer::getTargetTrans()const{return targetTrans;}
vector<float> layer::getTargetRots()const{return targetRots;}

float layer::getRot(int i){return rots[i];}
void layer::setRot(int i, float t_rot){rots[i] = t_rot; }
void layer::setTrans(int i, float t_trans){trans[i] = t_trans; }