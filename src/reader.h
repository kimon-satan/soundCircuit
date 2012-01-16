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
	
	reader();
	void update();
	void draw(ofRectangle vp);
	
	//getters and setters
	void setLayer(layer * t_layer);
	ofVec2f getPos();
	void setOscSender(ofxOscSender & t);
	
	private:
	
	void move();
	void moduloPosition();
	
	ofRectangle body, testBody;
	ofVec2f direction;
	ofVec2f world_dims;
	layer * currentLayer;
	ofxOscSender sender;
	
	float speed, incr;
	bool isStuck;
	
};

#endif

