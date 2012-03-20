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

int reader::tCounter = 0;

reader::reader(){
	
	body.set(0,0,WORLD_UNIT * 21, WORLD_UNIT * 21);
	mSpeed = 200;
	maxSpeed = 600;
	direction.set(1,0);
	isStuck =false;
	pDir.set(1,0);
	currentNodeIndex = -99;
	currentBlipIndex = -99;
	isSelected = false;
	isAdjusting = false;
	
}

void reader::update(){
	
	move();
	handleBlips();
	
}



void reader::moduloPosition(){
	
	ofVec2f t_dims = currentLayer->getWorldDims();
	ofVec2f t_pos(body.x, body.y);
	t_pos = utils::moduloPoint(t_pos, t_dims);
	body.x = t_pos.x;
	body.y = t_pos.y;

}

void reader::move(){
	
	mIncrement = mSpeed * WORLD_UNIT/ofGetFrameRate();
	
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
			
			nodeFound = true;
			
			if(currentNodeIndex != it->getIndex()){
				
				ofVec2f t_dir(nextDirection(direction, it->getNowSockets()));
				
				if(t_dir != ofVec2f(0,0)){
		
					currentNodeIndex = it->getIndex();
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
			
		}
	}
	
	if(!nodeFound)currentNodeIndex = -99;
	
	
		
}

void reader::handleBlips(){

	vector<blip> * t_blips = currentLayer->getBlipsRef();
	
	if(currentBlipIndex != -99){ //first kill old blip if reader not inside
		vector<blip>::iterator it = find_if(t_blips->begin(), t_blips->end(),bind2nd(blipIndex(),currentBlipIndex));
		if(!it->getInside(ofVec2f(body.x,body.y)) && !testBody.inside(it->getStartPos()))blipOff(it);
	}
	
	for(vector<blip>::iterator it = t_blips->begin(); it != t_blips->end(); it++){
		
		if(it->getInside(ofVec2f(body.x,body.y)) || testBody.inside(it->getStartPos())){
			
			if(currentBlipIndex == -99){
				
				currentBlipIndex = it->getIndex();
				
				it->addOccupant(); 
				it->react(mIncrement);
				
				blipPreset p = it->getPreset();
				
				ofxOscMessage m;
				
				m.setAddress("/blipOn");
				m.addIntArg(index);
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
		}
		
	}
}


void reader::blipOff(int tIndex){
	
	vector<blip> * t_blips = currentLayer->getBlipsRef();
	vector<blip>::iterator it = find_if(t_blips->begin(), t_blips->end(),bind2nd(blipIndex(), tIndex));
	if(it == t_blips->end())return;
	blipOff(it);
	
}

void reader::blipOff(vector<blip>::iterator it){

	if(currentBlipIndex == it->getIndex()){				
		
		it->subtractOccupant();
		
		currentBlipIndex = -99;
		blipPreset p = it->getPreset();
		
		if(p.getEnvType() == ENV_ASR){
			ofxOscMessage m;
			
			m.setAddress("/blipOff");
			m.addIntArg(index);
			m.addIntArg(it->getIndex());
			
			sender->sendMessage(m);
		}
		
	}
}

ofVec2f reader::nextDirection(ofVec2f c_dir, vector<bool> t_bools){
	
	
	vector<ofVec2f> available;
	
	for(int i = 0; i < 4 ; i++){
		if(i != node::getSocketIndex(-c_dir) && t_bools[i] != false){
			available.push_back(node::getSocketDirection(i));
		}
	}
	
	if(available.size() == 0){
		return ofVec2f(0,0);
	}else if(available.size() == 1){
		lDir.set(c_dir);
		return available[0];
		
	}
	
	int choice = floor(ofRandom(0, 1) * available.size());
	return available[choice];
	
	
}


void reader::draw(){
	
	ofPushMatrix();
	glDepthFunc(GL_ALWAYS);
	ofSetRectMode(OF_RECTMODE_CENTER);
	
	glTranslatef(body.x,body.y, 0);
	
	ofNoFill();
	ofSetColor(0);
	ofRect(0,0,body.width, body.height);
	
	if(isAdjusting){
		
		float radius = WORLD_UNIT * 30;
		ofCircle(0, 0, radius);
		
		ofPushStyle();
		ofFill();
		ofEnableAlphaBlending();
		ofSetColor(255, 0, 0, 100);

		ofBeginShape();
			
			ofVertex(0,0);
			ofVec2f o_pos(0, -radius);
			ofVertex(o_pos.x, o_pos.y);
			int degs = 360  * mSpeed/maxSpeed;
			for(int i = 0; i < degs; i ++){
				o_pos.rotate(1);
				ofVertex(o_pos.x, o_pos.y);
			}
			
			ofVertex(0, 0);
		
		ofEndShape(true);
		
		ofDisableAlphaBlending();
		ofPopStyle();
		
	}else if(isSelected){
		ofPushStyle();
		ofFill();
		ofEnableAlphaBlending();
		ofSetColor(255, 0, 0, 100);
		ofRect(0, 0, body.width + WORLD_UNIT * 5, body.height + WORLD_UNIT * 5);
		ofPopStyle();
	}
	
	
	ofSetRectMode(OF_RECTMODE_CORNER);
	glDepthFunc(GL_LESS);
	ofPopMatrix();
	
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

void reader::beginAdjust(){

	isAdjusting = true;
	
} 

void reader::adjust(ofVec2f w_pos){
	
	ofVec2f t_dir(w_pos - ofVec2f(body.x, body.y));
	float ang = t_dir.angle(ofVec2f(0,1));
	if(ang < 0){
		ang = 180 + abs(ang);
	}else{
		ang = 180 - abs(ang);
	}
	mSpeed = maxSpeed * ang/360;
	
}

void reader::endAdjust(){

	isAdjusting = false;

}

void reader::aquireIndex(){
	
	index = tCounter;
	tCounter += 1;
	
}

bool reader::getInside(ofVec2f pos){
	
	ofRectangle selectBody;
	selectBody.setFromCenter(body.x, body.y, body.width, body.height);
	isSelected = selectBody.inside(pos.x, pos.y);
	
	return isSelected;

}

void reader::endCurrentBlips(){

	if(currentBlipIndex >= 0)blipOff(currentBlipIndex);
	
}

//getters and setters

bool reader::getIsNewDirection(){return isNewDirection;}
void reader::setLayer(layer * t_layer){currentLayer = t_layer;}
ofVec2f reader::getPos(){return ofVec2f(body.x,body.y);}
void reader::setPos(ofVec2f t_pos){body.x = t_pos.x; body.y = t_pos.y;}
void reader::setOscSender(ofxOscSender * t){sender = t;}
ofVec2f reader::getDirection(){return direction;}
void reader::setDirection(ofVec2f dir){direction = dir;}
float reader::getIncrement(){return mIncrement;}
void reader::setIsSelected(bool t){isSelected = t;}
bool reader::getIsAdjusting(){return isAdjusting;}

void reader::setSpeed(float t){mSpeed = t; mSpeed = max(1.0f,mSpeed); mSpeed = min(maxSpeed, mSpeed);}
float reader::getSpeed(){return mSpeed;}
int reader::getIndex()const{return index;}
