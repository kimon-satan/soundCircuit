/*
 *  segment.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 04/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "segment.h"


int segment::sCounter = 0;

segment::segment(){
	
	isValid = false;
	isSelected  = false;
	index = -99;

}

bool segment::getInside(ofVec2f t_pos){
	
	if(!isWrapped){
		return (testArea.inside(t_pos));
	}else{
		if(testArea.inside(t_pos))return true;
		if(wrapTestArea.inside(t_pos))return true;
		return false;
	}
}

void segment::setSelected(bool t_select, ofVec2f t_pos){

	isSelected = t_select;
	
	if(isSelected){	
		if(direction.x > 0){
			selectPos.set(t_pos.x, startPos.y);
		}else{
			selectPos.set(startPos.x, t_pos.y);
		}
	}

}

void segment::aquireIndex(){
	
	index = sCounter;
	sCounter += 1;
	 
}

//getters and setters
void segment::setStartPos(ofVec2f t_pos){startPos = t_pos;}
ofVec2f segment::getStartPos(){return startPos;}
void segment::setEndPos(ofVec2f t_pos){endPos = t_pos;}
ofVec2f segment::getEndPos(){return endPos;}

void segment::setLength(float t_length){length = t_length;}
float segment::getLength(){return length;}
bool segment::getIsWrapped(){return isWrapped;}
void segment::setIsWrapped(bool t){isWrapped = t;}
void segment::setDirection(ofVec2f t_dir){direction = t_dir;}
ofVec2f segment::getDirection(){return direction;}

ofRectangle segment::getWrapTestArea(){return wrapTestArea;}
ofRectangle segment::getTestArea(){return testArea;}
void segment::setWrapTestArea(ofRectangle t){wrapTestArea.set(t);}
void segment::setTestArea(ofRectangle t){testArea.set(t);}

void segment::setIsValid(bool t_valid){isValid = t_valid;}
bool segment::getIsValid(){return isValid;}

bool segment::getIsSelected(){return isSelected;}
ofVec2f segment::getSelectPos(){return selectPos;}

int segment::getIndex()const{return index;}