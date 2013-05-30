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
        
        nowSockets_n.push_back(socket(i,getSocketDirection(i),false,false,false,false));
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


void node::openSocket(ofVec2f t_dir, int mode){
	
	openSocket(getSocketIndex(t_dir),mode);

}

void node::closeSocket(ofVec2f t_dir, int mode){

	closeSocket(getSocketIndex(t_dir), mode);
	
}

void node::openSocket(int t, int mode){
	
    if(nowSockets_n[t].available){
        
        switch(mode){
                
            case SM_ALL:
                nowSockets_n[t].left = true;
                nowSockets_n[t].straight = true;
                nowSockets_n[t].right = true;
            break;
                
            case SM_LEFT:
                nowSockets_n[t].left = true;
            break;
                
            case SM_STRAIGHT:
                nowSockets_n[t].straight = true;
            break;
                
            case SM_RIGHT:
                nowSockets_n[t].right = true;
            break;
                
                
        }
        
    }
	
}

void node::closeSocket(int t, int mode){
	
    if(nowSockets_n[t].available){
        
        switch(mode){
                
            case SM_ALL:
                nowSockets_n[t].left = false;
                nowSockets_n[t].straight = false;
                nowSockets_n[t].right = false;
                break;
                
            case SM_LEFT:
                nowSockets_n[t].left = false;
                break;
                
            case SM_STRAIGHT:
                nowSockets_n[t].straight = false;
                break;
                
            case SM_RIGHT:
                nowSockets_n[t].right = false;
                break;
                
                
        }
        
    }

	
}

void node::addSocket(ofVec2f t_dir){
	
	int i = getSocketIndex(t_dir);
			
	if(!nowSockets_n[i].available){
		nowSockets_n[i].available = true;
		numSockets += 1;
	}
	
	if(numSockets > 2){isShown = true;}
	
}

void node::removeSocket(ofVec2f t_dir){
	
	int i = getSocketIndex(t_dir);
		
	if(nowSockets_n[i].available){
		nowSockets_n[i].available = false;
		nowSockets[i] = false;
        
        nowSockets_n[i].left = false;
        nowSockets_n[i].straight = false;
        nowSockets_n[i].right = false;
        
		numSockets -= 1;
	}
	
	if(numSockets < 3){isShown = false;}
	
}

bool node::getSuperfluous(){
	
	if(numSockets < 1)return true;
	
	for(int i = 0; i< 2; i ++){
		if(nowSockets_n[i].available == true && nowSockets_n[i+2].available == true){
			if(nowSockets_n[i+1].available == false && nowSockets_n[(i+3)%4].available == false){
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

void node::setSelected(bool t){isSelected = t;}
int node::getIndex(){return index;}
bool node::getIsSelected(){return isSelected;}

vector<bool> node::getNowSockets(){return nowSockets;}

bool node::getNowSocket(ofVec2f t_dir){return nowSockets[getSocketIndex(t_dir)];}
bool node::getNowSocket(int i){return nowSockets[i];}


vector<socket> node::getNowSockets_n(){
    return nowSockets_n;
}
socket node::getNowSocket_n(ofVec2f t_dir){
    return nowSockets_n[getSocketIndex(t_dir)];
}
socket node::getNowSocket_n(int i){
    return nowSockets_n[i];
}

vector<bool> node::getAllSockets(){return allSockets;}
bool node::getAllSocket(ofVec2f t_dir){return allSockets[getSocketIndex(t_dir)];}

bool node::getIsShown(){return isShown;}

bool node::getIsAdjusting(){return isAdjusting;}
void node::setIsAdjusting(bool t){isAdjusting = t;}