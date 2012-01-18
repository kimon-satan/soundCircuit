/*
 *  node.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 02/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "node.h"

int node::tCounter = 0;

node::node(ofVec2f t_pos) : pos(t_pos){
	
	for(int i = 0; i < 4; i ++)socketBools.push_back(false);
	
	isSelected = false;
	
	socketDirections.push_back(ofVec2f(0,-1)); // NORTH
	socketDirections.push_back(ofVec2f(1,0)); // EAST
	socketDirections.push_back(ofVec2f(0,1)); // SOUTH
	socketDirections.push_back(ofVec2f(-1,0)); // WEST
	
	aquireIndex();

}

void node::aquireIndex(){
	
	index = tCounter;
	tCounter += 1;

}



void node::openSocket(ofVec2f t_dir){
	
	for(int i = 0; i < 4 ; i++){
		if(socketDirections[i] == t_dir){
			socketBools[i] = true;
			break;
		}
	}
	

}

ofVec2f node::getNextDirection(ofVec2f c_dir){
	
	vector<ofVec2f> available;
	 
	for(int i = 0; i < 4 ; i++){
		if(socketDirections[i] != -c_dir && socketBools[i] != false){
			available.push_back(socketDirections[i]);
		}
	}
	
	if(available.size() == 0){
		return ofVec2f(0,0);
	}else if(available.size() == 1){
		return available[0];
	}else if(available.size() > 1){
		for(int i = 0; i < available.size() ; i++){
			if(available[i] == c_dir){
				available.erase(available.begin() + i);
				break;
			}
		}
		int choice = floor(ofRandom(0, 1) * available.size());
		return available[choice];
	}
	
	
}


bool node::getSuperfluous(){
	
	for(int i = 0; i< 2; i ++){
		if(socketBools[i] == true && socketBools[i+2] == true){
			if(socketBools[i+1] == false && socketBools[(i+3)%4] == false){
				return true;
			}
		}
	}
	
	return false;
}

//getters and setters
ofVec2f node::getPos(){return pos;}
void node::setPos(ofVec2f p){pos = p;}
void node::setIsActive(bool t){isActive = t;}
bool node::getIsActive(){return isActive;}
void node::setSelected(bool t){isSelected = t;}
int node::getIndex(){return index;}
bool node::getIsSelected(){return isSelected;}