/*
 *  reader.cpp
 *  circuitAnim
 *
 *  Created by Simon Katan on 30/11/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "reader.h"
#include "layer.h"
#include "utils.h"

reader::reader(){
	
	body.set(0,0,WORLD_UNIT * 21, WORLD_UNIT * 21);
	mSpeed = WORLD_UNIT * 200;
	direction.set(1,0);
	isStuck =false;
	mode = READER_WANDER;
	pDir.set(1,0);
	
	socketDirections.push_back(ofVec2f(0,-1)); // NORTH
	socketDirections.push_back(ofVec2f(1,0)); // EAST
	socketDirections.push_back(ofVec2f(0,1)); // SOUTH
	socketDirections.push_back(ofVec2f(-1,0)); // WEST
	
	
}

void reader::update(){
	
	move();
	
}



void reader::moduloPosition(){
	
	ofVec2f t_dims = currentLayer->getWorldDims();
	ofVec2f t_pos(body.x, body.y);
	t_pos = utils::moduloPoint(t_pos, t_dims);
	body.x = t_pos.x;
	body.y = t_pos.y;

}

void reader::move(){
	
	mIncrement = mSpeed/ofGetFrameRate();
	
	if(!isStuck){
	body.x += direction.x * mIncrement;
	body.y += direction.y * mIncrement;
	moduloPosition();
	}
	
	vector<node> * t_nodes = currentLayer->getNodesRef(); 
	
	testBody.setFromCenter(body.x, body.y, mIncrement * 2, mIncrement * 2);
	
	bool nodeFound = false;
	isNewDirection = false;
	
	for(vector<node>::iterator it = t_nodes->begin(); it != t_nodes->end(); it++){
		if(testBody.inside(it->getPos())){ 
			
			if(!it->getIsActive()){
				
				ofVec2f t_dir(nextDirection(direction, it->getSockets()));
				nodeFound = true;
				
				if(t_dir != ofVec2f(0,0)){
					it->setIsActive(true);
					direction.set(t_dir);
					body.x = it->getPos().x;
					body.y = it->getPos().y;
					isStuck = false;
					isNewDirection = true;
					break;
				}else{
					
					isStuck = true;
					break;
				}
				
			}
			
		}else{
			it->setIsActive(false);
		}
	}
	
	isStuck = nodeFound;
	
	vector<blip> * t_blips = currentLayer->getBlipsRef();
	
	for(vector<blip>::iterator it = t_blips->begin(); it != t_blips->end(); it++){
	
		if(it->getInside(ofVec2f(body.x,body.y)) || testBody.inside(it->getStartPos())){
			if(it->react(mIncrement)){

				blipPreset p = it->getPreset();
				
				ofxOscMessage m;
				
				m.setAddress("/blipOn");
				m.addIntArg(it->getIndex());
				m.addStringArg(p.getSynthDef());
				m.addIntArg(p.getEnvType());
				m.addFloatArg(p.getAttackSecs()->abs_value);
				m.addFloatArg(p.getDecaySecs()->abs_value);
				
				for(int i = 0; i < p.getSoundParams()->size(); i++){
					m.addFloatArg(p.getSoundParam(i)->abs_value);
				}
				
				sender->sendMessage(m);
			}
			
		}else{
			if(it->getIsOccupied()){				
				it->setIsOccupied(false);
				
				blipPreset p = it->getPreset();
				
				if(p.getEnvType() == ENV_ASR){
					ofxOscMessage m;
					
					m.setAddress("/blipOff");
					m.addIntArg(it->getIndex());
					
					sender->sendMessage(m);
				}
				
			}
		}
	}
	
		
}

ofVec2f reader::nextDirection(ofVec2f c_dir, vector<bool> t_bools){
	
	
	vector<ofVec2f> available;
	
	for(int i = 0; i < 4 ; i++){
		if(socketDirections[i] != -c_dir && t_bools[i] != false){
			available.push_back(socketDirections[i]);
		}
	}
	
	if(available.size() == 0){
		return ofVec2f(0,0);
	}else if(available.size() == 1){
		lDir.set(c_dir);
		return available[0];
		
	}
	
	if(mode == READER_WANDER){
		
		for(int i = 0; i < available.size() ; i++){
			if(available[i] == c_dir){
				available.erase(available.begin() + i);
				break;
			}
		}
		
		int choice = floor(ofRandom(0, 1) * available.size());
		return available[choice];
	
	}else if(mode == READER_STRAIGHT){
			
		for(int i = 0; i < available.size() ; i++){
			if(available[i] == c_dir){
				return available[i];
			}
		}
		
		int choice = floor(ofRandom(0, 1) * available.size());
		return available[choice];
		
	}else if(mode == READER_PERSIST){
		
		for(int i = 0; i < available.size() ; i++){
			if(available[i] == pDir){
				available.erase(available.begin() + i);
				break;
			}
		}
			
		int choice = floor(ofRandom(0, 1) * available.size());
		return available[choice];
		
	}else if(mode == READER_LOOP){
		
		for(int i = 0; i < available.size() ; i++){
			if(available[i] == -lDir){
				lDir = c_dir;
				return available[i];
				break;
			}
		}
		
		int choice = floor(ofRandom(0, 1) * available.size());
		return available[choice];
			
	}else{		

		int choice = floor(ofRandom(0, 1) * available.size());
		return available[choice];
		
	}
	
	
	
}


void reader::draw(){
	
	ofPushMatrix();
	glDepthFunc(GL_ALWAYS);
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofNoFill();
	ofSetColor(0);
	ofRect(body);
	ofSetRectMode(OF_RECTMODE_CORNER);
	glDepthFunc(GL_LESS);
	ofPopMatrix();
	
}

void reader::incrementMode(){

	mode = e_readerMode(mode + 1);
	mode = e_readerMode((mode)%READER_COUNT);
	if(mode == READER_PERSIST)pDir.set(-direction);
	if(mode == READER_LOOP)lDir.set(direction);
}


string reader::getModeString(){
	
	string modeString = "";
	
	switch (mode) {
		case READER_FREE: modeString = "free"; break;
		case READER_PERSIST: modeString = "persist"; break;
		case READER_WANDER: modeString = "wander"; break;
		case READER_STRAIGHT: modeString = "straight"; break;
		case READER_LOOP: modeString = "loop"; break;
	}
	
	return modeString;
}


void reader::beginInsertion(ofVec2f t_point, ofVec2f t_dir){
	
	insertPoint.set(t_point);
	insertDir.set(t_dir);
	insertSize = 0;
	
}

void reader::resizeInsertion(float size){
	
	float diff = size - insertSize;
	insertSize = size;
	float testInsert = insertPoint.x * insertDir.x + insertPoint.y * insertDir.y;

	ofVec2f t_pos(body.x, body.y);
	float testPos = body.x * insertDir.x + body.y * insertDir.y;
	t_pos += (testPos > testInsert)? diff * insertDir: -diff * insertDir;
	body.x = t_pos.x; body.y = t_pos.y;
		

}


//getters and setters

bool reader::getIsNewDirection(){return isNewDirection;}
void reader::setLayer(layer * t_layer){currentLayer = t_layer;}
ofVec2f reader::getPos(){return ofVec2f(body.x,body.y);}
void reader::setPos(ofVec2f t_pos){body.x = t_pos.x; body.y = t_pos.y;}
void reader::setOscSender(ofxOscSender * t){sender = t;}
ofVec2f reader::getDirection(){return direction;}
float reader::getIncrement(){return mIncrement;}

void reader::setSpeed(float t){mSpeed = t;}
float reader::getSpeed(){return mSpeed;}

