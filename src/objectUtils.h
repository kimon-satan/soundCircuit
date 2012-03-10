/*
 *  objectUtils.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 12/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef objectUtils_h
#define objectUtils_h

#include "ofMain.h"
#include "segment.h"
#include "utils.h"

class objectUtils{
	
public:
	
	objectUtils();
	//getters and setters
	void setWorldDims(ofVec2f t_dims);
	ofVec2f getWorldDims()const;
	
protected:
	
	void limitStartPointFromMid(ofVec2f origin, vector<ofVec2f> & t_points, segment & s);
	void limitEndPointFromMid(ofVec2f origin, vector<ofVec2f> & t_points, segment & s, bool preserveLength = false);
	void limitEndPointFromStart(ofVec2f origin, vector<ofVec2f> & t_points, segment & s);
	
	vector<ofVec2f> getPlanarNeighbours(ofVec2f origin, vector<ofVec2f> &t_points);
	void sortNeighboursToSegment(ofVec2f origin, vector<ofVec2f> &t_points, segment & s);
	
	void makeSegment(ofVec2f w_pos, ofVec2f dir, segment & s);
	void updateTestAreas(segment & t);
	void quantizeDirection(ofVec2f & t_dir);
	
	ofVec2f world_dims;
	
	static float kTestSize;
	
};

#endif
