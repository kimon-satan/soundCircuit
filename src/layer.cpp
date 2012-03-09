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
	

	isScreenData = false;

}

void layer::update(){

	mReader.update();
	for(vector<blip>::iterator it = blips.begin(); it != blips.end(); it++)it->update();
	
	
}

void layer::draw(ofVec2f pos, ofRectangle roi, ofColor col){
	
	
	glPushMatrix();
	ofNoFill();

	glTranslatef(pos.x,pos.y,0); //centred coordinates
	
	//draw background for coordinate picking
	
	if(isScreenData){
		ofPushMatrix();
		ofTranslate(0, 0, 0);
		ofSetColor(col);
		ofFill();
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofRect(0,0,world_dims.x, world_dims.y);
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofNoFill();
		ofSetColor(0);
		ofPopMatrix();
	}

	
	//calculate area that will be viewed
	
	render(pos, roi);
	
	mReader.draw();
	
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



void layer::toggleScreenData(){isScreenData = !isScreenData;}

//getters and setters


vector<node> * layer::getNodesRef(){return &nodes;}
vector<blip> * layer::getBlipsRef(){return &blips;}

vector<node> layer::getNodes()const{return nodes;}
vector<blip> layer::getBlips()const{return blips;}
vector<segment> layer::getTracks()const{return tracks;}

reader * layer::getReaderRef(){return &mReader;}
reader layer::getReader()const{return mReader;}


