/*
 *  objectRenderer.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 05/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef objectRenderer_h
#define objectRenderer_h

#include "objectManager.h"

class objectRenderer : public objectManager{

public:
	
	objectRenderer();
	
	void draw(ofRectangle t_view);

	void drawTracks();
	void drawBlips();
	void drawNodes();
	
	//getters and setters
	void toggleTrackData();
	void toggleNodeData();
	void toggleBlipData();
	
private:
	
	bool checkIsVisible(ofVec2f a, ofVec2f b, ofVec2f t_dir);
	bool checkIsVisible(vector<ofVec2f> t_corners);
	void drawTrack(segment * t);
	
	ofRectangle viewPort;
	bool isTrackData;
	bool isNodeData;
	bool isBlipData;
	
};

#endif