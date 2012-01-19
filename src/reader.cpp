/*
 *  reader.cpp
 *  circuitAnim
 *
 *  Created by Simon Katan on 30/11/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "reader.h"

reader::reader(){
	
	body.set(0,0,21,21);
	speed = 200;
	direction.set(1,0);
	isStuck =false;

}

void reader::update(){
	
	move();
	
	
}


void reader::moduloPosition(){
	
	ofVec2f t_dims = currentLayer->getDims();
	
	//reader position wrapping
	if(body.x > t_dims.x/2){
		body.x = -t_dims.x/2;
	}else if(body.x < -t_dims.x/2){
		body.x = t_dims.x/2;
	}

	if(body.y > t_dims.y/2){
		body.y = -t_dims.y/2;
	}else if(body.y < -t_dims.y/2){
		body.y = t_dims.y/2;
	}
}

void reader::move(){
	
	incr = speed/ofGetFrameRate();
	
	if(!isStuck){
	body.x += direction.x * incr;
	body.y += direction.y * incr;
	moduloPosition();
	}
	
	vector<node> * t_nodes = currentLayer->getNodes(); 
	
	testBody.setFromCenter(body.x, body.y, incr * 2, incr * 2);
	
	bool nodeFound = false;
	
	for(vector<node>::iterator it = t_nodes->begin(); it != t_nodes->end(); it++){
		if(testBody.inside(it->getPos())){ 
			
			if(!it->getIsActive()){
				
				ofVec2f t_dir(it->getNextDirection(direction));
				nodeFound = true;
				
				if(t_dir != ofVec2f(0,0)){
					it->setIsActive(true);
					direction.set(t_dir);
					body.x = it->getPos().x;
					body.y = it->getPos().y;
					isStuck = false;
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
	
	vector<blip> * t_blips = currentLayer->getBlips();
	
	for(vector<blip>::iterator it = t_blips->begin(); it != t_blips->end(); it++){
	
		if(it->getInside(ofVec2f(body.x,body.y))){
			if(it->react()){

				blipPreset p = it->getPreset();
				
				ofxOscMessage m;
				
				m.setAddress("/blipOn");
				m.addIntArg(it->getIndex());
				m.addStringArg(p.getSynthDef());
				m.addIntArg(p.getEnvType());
				m.addFloatArg(p.getAttackSecs().abs_value);
				m.addFloatArg(p.getDecaySecs().abs_value);
				
				for(int i = 0; i < p.getSoundParams().size(); i++){
					m.addFloatArg(p.getSoundParam(i).abs_value);
				}
				
				sender.sendMessage(m);
			}
			
		}else{
			if(it->getIsOccupied()){				
				it->setIsOccupied(false);
				
				blipPreset p = it->getPreset();
				
				if(p.getEnvType() == ENV_ASR){
					ofxOscMessage m;
					
					m.setAddress("/blipOff");
					m.addIntArg(it->getIndex());
					
					sender.sendMessage(m);
				}
				
			}
		}
	}
		
}


void reader::draw(ofRectangle vp){
	
	glPushMatrix();
	glTranslatef(ofGetScreenWidth()/2, ofGetScreenHeight()/2, 0);
	glPushMatrix();
	glTranslatef(-vp.x, -vp.y,0); //centred coordinates
	
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofNoFill();
	ofSetColor(0);
	ofRect(body);
	
	glPopMatrix();
	glPopMatrix();
	
	ofSetRectMode(OF_RECTMODE_CORNER);
	
	
}

//getters and setters

void reader::setLayer(layer * t_layer){currentLayer = t_layer;}
ofVec2f reader::getPos(){return ofVec2f(body.x,body.y);}
void reader::setOscSender(ofxOscSender & t){sender = t;}

