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

enum e_setType{
	
	PSET_FIXED,
	PSET_USERA,
	PSET_USERB,
	PSET_MAP,
	PSET_RANDOM
	
};

struct paramAttributes{
	
	paramAttributes(){
		name = "emptyAttribute";
		abs_value = 0;
		min_val = 0;
		max_val = 1;
		setType = PSET_FIXED;
	}
	
	string name;
	float abs_value, min_val, max_val;
	e_setType setType;
};



class baseBlipDraw{

public:
	
	baseBlipDraw();
	
	virtual void draw(bool isWrapped = false){};
	virtual void update(){};
	virtual void drawPreview(){};
	
	//getters and setters
	void setBlipParams(ofVec2f & t_dir, ofVec2f & t_sp, ofVec2f & t_ep, float & t_l);
	void setPresetParams(vector<float> t_params);
	void setTimeParams(bool & t_active, float & t_envVal);

	ofRectangle getDrawRect();
	ofRectangle getWrapDrawRect();
	void setTestingRects(ofVec2f a, ofVec2f b, ofVec2f wa, ofVec2f wb);
	bool getIsDrawWrap();
	
protected:
	
	ofVec2f centre, wrapCentre;
	ofVec2f direction, startPos, endPos;
	float length, angle;
	ofRectangle drawRect, wrapDrawRect;
	vector<float> params;
	float envVal;
	bool isActive;
	bool isDrawWrap;

};

#endif