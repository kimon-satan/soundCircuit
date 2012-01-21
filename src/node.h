/*
 *  node.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 02/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef node_h
#define node_h

#include "ofMain.h"


class node{

public:
	
	node(ofVec2f t_pos);
	
	void openSocket(ofVec2f t_dir);

	
	bool getSuperfluous();
	int getIndex();
	void aquireIndex();
	
	
	//getters and setters
	
	ofVec2f getPos();
	void setPos(ofVec2f p);
	void setSelected(bool t);
	bool getIsSelected();
	void setIsActive(bool t);
	bool getIsActive();
	vector<bool> getSockets();

private:
	
	ofVec2f pos;
	vector<bool> socketBools;
	vector<ofVec2f> socketDirections;
	bool isSelected, isActive;
	int index;
	static int tCounter;

	
	
	
};

#endif
