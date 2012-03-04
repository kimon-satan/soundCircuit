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
	
	void update();
	void draw();

	void draw(ofRectangle& vp, bool isDummy = false);
	void drawPreview(ofRectangle vp);
	
	void selectSomething(ofVec2f w_pos);
	void deselectAll();
	
	void beginInsertion(ofVec2f t_pos, ofVec2f t_dir);
	void resizeInsertion(float size);
	void endInsertion();
	
	//getters and setters

	vector<node> * getNodes();
	vector<blip> * getBlips();

	void toggleScreenData();
	reader * getReader();
	

private:
	
	reader mReader;
	bool isScreenData;
	
	
	
};

#endif