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
#include "layer.h"
#include "ofxOsc.h"


class reader{
	
	public:
	
	enum e_readerMode{
		
		READER_STRAIGHT,
		READER_PERSIST,
		READER_LOOP,
		READER_FREE,
		READER_WANDER,
		READER_COUNT
	};
	
	reader();
	bool update();
	void draw(ofRectangle vp);
	
	void incrementMode();
	
	//getters and setters
	void setLayer(layer * t_layer);
	ofVec2f getPos();
	void setOscSender(ofxOscSender * t);
	string getModeString();
	ofVec2f getDirection();
	float getIncrement();
	
	private:
	
	bool move();
	void moduloPosition();
	
	ofVec2f nextDirection(ofVec2f t_dir, vector<bool> t_bools);

	
	ofRectangle body, testBody;
	ofVec2f direction, oldDirection;
	ofVec2f pDir, lDir;
	vector<ofVec2f>socketDirections;
	ofVec2f world_dims;
	layer * currentLayer;
	ofxOscSender * sender;
	e_readerMode mode;
	
	float speed, incr;
	bool isStuck;
	
};

#endif

