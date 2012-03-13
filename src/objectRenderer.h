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
	
	void render(ofVec2f t_pos, ofRectangle roi);

	void drawTracks();
	void drawBlips();
	void drawNodes();
	void drawSelected();
	
	//getters and setters
	void toggleTrackData();
	void toggleNodeData();
	void toggleBlipData();
	
private:
	
	void drawTrack(segment * t);
	bool checkIsVisible(ofVec2f a, ofVec2f b, ofVec2f t_dir);
	bool checkIsVisible(vector<ofVec2f> t_corners);
	bool checkIsVisible(ofVec2f t_pos);
	
	ofRectangle viewPort;
	bool isTrackData;
	bool isNodeData;
	bool isBlipData;
	ofVec2f center;
	
	vector<ofVec2f> selected;
	static float kCircleRadius;
	
};

#endif