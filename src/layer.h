/*
 *  layer.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 04/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef layer_h
#define layer_h

#include "ofMain.h"
#include "objectRenderer.h"
#include "reader.h"

class layer{

public:
	
	layer();
	
	void update();
	void draw();

	void draw(ofRectangle& vp, bool isDummy = false);
	void drawPreview(ofRectangle vp);
	
	void selectSomething(ofVec2f w_pos);
	void deselectAll();
	
	void expand();
	
	//getters and setters
	void setDims(ofVec2f t_dims);
	ofVec2f getDims();
	vector<node> * getNodes();
	vector<blip> * getBlips();
	objectRenderer * getObjectRenderer();
	void toggleScreenData();
	reader * getReader();
	

private:
	
	ofVec2f world_dims;
	objectRenderer mObjectRenderer;
	reader mReader;
	
	vector<node> nodes;
	vector<segment> tracks;
	vector<blip> blips;
	
	bool isScreenData;
	
};

#endif