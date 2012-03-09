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


class layer : public objectRenderer{

public:
	
	layer();
	layer(const layer &src);
	
	void setup();
	void update();
	void draw(ofVec2f pos, ofRectangle roi, ofColor col);
	
	void selectSomething(ofVec2f w_pos);
	void deselectAll();
	
	void beginInsertion(ofVec2f t_pos, ofVec2f t_dir);
	void resizeInsertion(float size);
	void endInsertion();

	
	//getters and setters

	vector<node> * getNodesRef();
	vector<blip> * getBlipsRef();
	
	vector<node> getNodes()const;
	vector<blip> getBlips()const;
	vector<segment> getTracks() const;
	
	void toggleScreenData();
	reader * getReaderRef();
	reader getReader()const;
	
private:
	
	reader mReader;
	bool isScreenData;

	
	ofColor testCol;
	
};

#endif