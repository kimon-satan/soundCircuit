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
ofVec2f node::socketDirections[4] = {ofVec2f(0,-1),ofVec2f(1,0),ofVec2f(0,1),ofVec2f(-1,0)};

//static method
int node::getSocketIndex(ofVec2f t_dir){

	for(int i = 0; i < 4; i++){
		if(t_dir == socketDirections[i])return i;
	}
	
	return -99;
}

ofVec2f node::getSocketDirection(int i){return socketDirections[i];}

node::node(ofVec2f t_pos) : pos(t_pos){
	
	for(int i = 0; i < 4; i ++){
		nowSockets.push_back(false);
		allSockets.push_back(false);
	}
	
	isSelected = false;
	isShown = false;
	isAdjusting = false;
	numSockets = 0;
	numOpen = 0;
	
	aquireIndex();


}

void node::aquireIndex(){
	
	index = tCounter;
	tCounter += 1;

}


void node::openSocket(ofVec2f t_dir){
	
	openSocket(getSocketIndex(t_dir));

}

void node::closeSocket(ofVec2f t_dir){

	closeSocket(getSocketIndex(t_dir));
	
}

void node::openSocket(int t){
	
	if(allSockets[t])nowSockets[t] = true;
	
}

void node::closeSocket(int t){
	
	if(allSockets[t])nowSockets[t] = false;
	
}

void node::addSocket(ofVec2f t_dir){
	
	int i = getSocketIndex(t_dir);
			
	if(!allSockets[i]){
		allSockets[i] = true;
		numSockets += 1;
	}
	
	if(numSockets > 2){isShown = true;}
	
}

void node::removeSocket(ofVec2f t_dir){
	
	int i = getSocketIndex(t_dir);
		
	if(allSockets[i]){
		allSockets[i] = false;
		nowSockets[i] = false;
		numSockets -= 1;
	}
	
	if(numSockets < 3){isShown = false;}
	
}

bool node::getSuperfluous(){
	
	if(numSockets < 1)return true;
	
	for(int i = 0; i< 2; i ++){
		if(allSockets[i] == true && allSockets[i+2] == true){
			if(allSockets[i+1] == false && allSockets[(i+3)%4] == false){
				return true;
			}
		}
	}
	
	return false;
}

//getters and setters

int node::getNumSockets(){return numSockets;}
ofVec2f node::getPos(){return pos;}
void node::setPos(ofVec2f p){pos = p;}

//not needed

/*void node::addReader(reader * r){readerPtrs.push_back(r);}

bool node::getReaderPresent(reader * r){
	for(int i = 0; i < readerPtrs.size(); i++)if(readerPtrs[i] == r)return true;
	return false;
}

void node::removeReader(reader * r){

	vector<reader*>::iterator it;
	it = remove(readerPtrs.begin(), readerPtrs.end(), r);
	readerPtrs.erase(it, readerPtrs.end());
}*/

void node::setSelected(bool t){isSelected = t;}
int node::getIndex(){return index;}
bool node::getIsSelected(){return isSelected;}

vector<bool> node::getNowSockets(){return nowSockets;}

bool node::getNowSocket(ofVec2f t_dir){return nowSockets[getSocketIndex(t_dir)];}
bool node::getNowSocket(int i){return nowSockets[i];}

vector<bool> node::getAllSockets(){return allSockets;}
bool node::getAllSocket(ofVec2f t_dir){return allSockets[getSocketIndex(t_dir)];}

bool node::getIsShown(){return isShown;}

bool node::getIsAdjusting(){return isAdjusting;}
void node::setIsAdjusting(bool t){isAdjusting = t;}