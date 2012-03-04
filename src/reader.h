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
class layer;

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
	void update();
	void draw();
	
	void incrementMode();

	void beginInsertion(ofVec2f t_point, ofVec2f t_dir);
	void resizeInsertion(float size);

	//getters and setters
	void setLayer(layer * t_layer);
	ofVec2f getPos();
	void setPos(ofVec2f t_pos);
	void setOscSender(ofxOscSender * t);
	string getModeString();
	ofVec2f getDirection();
	int getIncrement();
	void setSpeed(int t);
	int getSpeed();
	bool getIsNewDirection();
	
	private:
	
	void move();
	void moduloPosition();
	
	ofVec2f nextDirection(ofVec2f t_dir, vector<bool> t_bools);

	
	ofRectangle body, testBody;
	ofVec2f direction, oldDirection;
	ofVec2f pDir, lDir;
	vector<ofVec2f>socketDirections;
	layer * currentLayer;
	ofxOscSender * sender;
	e_readerMode mode;
	
	ofVec2f insertPoint;
	ofVec2f insertDir;
	float insertSize;

	
	int mSpeed, mIncrement;
	bool isStuck, isNewDirection;
	
};

#endif

