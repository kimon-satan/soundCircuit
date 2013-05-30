/*
 *  reader.h
 *  circuitAnim
 *
 *  Created by Simon Katan on 30/11/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef reader_h
#define reader_h

#include "ofMain.h"
#include "ofxOsc.h"
#include "blip.h"
#include "node.h"

class layer;



class reader{
	
	public:

	reader();
	void update();
	void draw();

	void beginInsertion(ofVec2f t_point, ofVec2f t_dir, float t_size);
	void resizeInsertion(float size);
	
	void beginAdjust();
	void adjust(ofVec2f w_pos);
	void endAdjust();
	
	bool getInside(ofVec2f pos);
	void endCurrentBlips();
	
	//getters and setters
	void setIsSelected(bool t);
	bool getIsAdjusting();
	void setLayer(layer * t_layer);
	ofVec2f getPos();
	void setPos(ofVec2f t_pos);
	void setOscSender(ofxOscSender * t);
	string getModeString();
	ofVec2f getDirection();
	void setDirection(ofVec2f dir);
	float getIncrement();
	void setSpeed(float t);
	float getSpeed();
	bool getIsNewDirection();
	
	int getIndex()const;
	void aquireIndex();
	
	private:
	
	void move();
	void moduloPosition();
	
	ofVec2f nextDirection(ofVec2f t_dir, vector<socket> t_sockets);
	void handleBlips();
	void blipOff(vector<blip>::iterator it);
	void blipOff(int tIndex);

	
	ofRectangle body, testBody;
	ofVec2f direction, oldDirection;
	ofVec2f pDir, lDir;
	layer * currentLayer;
	ofxOscSender * sender;
	
	ofVec2f insertPoint;
	ofVec2f insertDir;
	float insertSize;
	
	float mSpeed, mIncrement, maxSpeed;
	bool isStuck, isNewDirection, isSelected, isAdjusting;
	
	int currentNodeIndex;
	int currentBlipIndex;
	
	int index;
	static int tCounter;
	
};

#endif

