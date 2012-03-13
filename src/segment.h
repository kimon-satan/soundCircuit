/*
 *  segment.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 04/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef segment_h
#define segment_h

#include "ofMain.h"

class segment{

public:
	
	segment();
	bool getInside(ofVec2f t_pos);
	void setSelected(bool t_select, ofVec2f t_pos = ofVec2f(0,0));
	
	//getters and setters
	virtual void setStartPos(ofVec2f t_pos);
	ofVec2f getStartPos();
	void setEndPos(ofVec2f t_pos);
	ofVec2f getEndPos();
	
	virtual void setDirection(ofVec2f t_dir);
	ofVec2f getDirection();
	virtual void setLength(float t_length);
	float getLength();
	
	bool getIsWrapped();
	void setIsWrapped(bool t);
	
	void setTestArea(ofRectangle t);
	void setWrapTestArea(ofRectangle t);
	ofRectangle getTestArea();
	ofRectangle getWrapTestArea();
	
	void setIsValid(bool t_valid);
	bool getIsValid();
	int getIndex()const;
	virtual void aquireIndex();
	
	bool getIsSelected();
	ofVec2f getSelectPos();
	
protected:
	
	ofRectangle testArea;
	ofRectangle wrapTestArea;
	bool isWrapped, isValid, isSelected;
	ofVec2f startPos, endPos, direction, selectPos;
	float length;
	int index;
	
private:
	
	static int sCounter;

};

#endif