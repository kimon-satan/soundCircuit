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


class baseBlipDraw{

public:
	
	virtual void draw(bool isWrapped = false){};
	virtual void update(){};
	virtual void drawPreview(){};
	
	//getters and setters
	void setBlipParams(ofVec2f & t_dir, ofVec2f & t_sp, ofVec2f & t_ep, float & t_l);
	void setPresetParams(vector<float> t_params);
	void setTimeParams(bool & t_active, float & t_envVal);
	
protected:
	
	ofVec2f centre, wrapCentre;
	ofVec2f direction, startPos, endPos;
	float length, angle;
	vector<float> params;
	float envVal;
	bool isActive;

};

#endif