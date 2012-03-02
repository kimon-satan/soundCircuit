/*
 *  layer.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 04/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "layer.h"

layer::layer(){

	mObjectRenderer.setNodes(&nodes);
	mObjectRenderer.setTracks(&tracks);
	mObjectRenderer.setBlips(&blips);
	isScreenData = false;
	mReader.setLayer(this);
	mObjectRenderer.setIncr(mReader.getIncrement());

}

void layer::update(){

	mReader.update();
	for(vector<blip>::iterator it = blips.begin(); it != blips.end(); it++)it->update();
	

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
	
	mObjectRenderer.draw(viewable);
	
	mReader.draw();
	
	glPopMatrix();
	
	glPopMatrix();
}


void layer::selectSomething(ofVec2f w_pos){

	mObjectRenderer.deselectTracks();
	mObjectRenderer.deselectNodes();
	if(!mObjectRenderer.selectNode(w_pos)){
		mObjectRenderer.selectTrackPoint(w_pos);
	}

}

void layer::deselectAll(){

	mObjectRenderer.deselectNodes();
	mObjectRenderer.deselectTracks();
}


void layer::expand(){
	
	ofVec2f t_dims = world_dims + 10;
	mReader.resize(t_dims);
	mObjectRenderer.resize(t_dims);
	world_dims = t_dims;
	

}

//getters and setters

void layer::setDims(ofVec2f t_dims){
	
	world_dims = t_dims;
	mObjectRenderer.setWorldDims(t_dims);
	
}

ofVec2f layer::getDims(){return world_dims;}
vector<node> * layer::getNodes(){return &nodes;}
vector<blip> * layer::getBlips(){return &blips;}
objectRenderer * layer::getObjectRenderer(){return & mObjectRenderer;}
void layer::toggleScreenData(){isScreenData = !isScreenData;}
reader * layer::getReader(){return &mReader;}