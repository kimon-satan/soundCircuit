/*
 *  blipDraw.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 28/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef baseBlipDraw_h
#define baseBlipDraw_h

#include "ofMain.h"
#include "paramAttributes.h"

class baseBlipDraw{

public:
	
	baseBlipDraw();
	
	virtual void setup(ofVec2f t_dims);
	virtual void setup(ofVec2f t_dims, baseBlipDraw * t_draw);
	virtual void draw(int t_wrap = 0){};
	virtual void update(){};
	virtual void drawPreview(){};
	
	void setCorners(vector<ofVec2f> & t_corners, ofVec2f t_centre, ofVec2f t_dims, float t_angle);
	void setWrapData(ofVec2f t_dims, float t_angle);
	
	//getters and setters
	void setBlipParams(ofVec2f & t_dir, ofVec2f & t_sp, ofVec2f & t_ep, float & t_l);
	void setDecayRatio(float decay, float postdecay);
	void setPresetParams(vector<float> t_params);
	void setTimeParams(bool  t_active, float  t_envVal, float  t_postVal);
	
	vector<ofVec2f>getCorners();
	vector<ofVec2f>getWrapXCorners();
	vector<ofVec2f>getWrapYCorners();
	
	bool getIsXWrapped();
	bool getIsYWrapped();
	
	ofVec2f getDirection();
	ofVec2f getStartPos();
	ofVec2f getEndPos();
	float getLength();
	
	
	
protected:
	
	ofVec2f world_dims, centre, wrapCoords;
	vector<ofVec2f> corners, wrapXcorners, wrapYcorners;
	ofVec2f wrapXCentre, wrapYCentre;
	ofVec2f direction, startPos, endPos;
	float length, angle;
	vector<float> params;
	float envVal, postVal, decayRatio;
	bool isActive, isXWrapped, isYWrapped;

};

#endif