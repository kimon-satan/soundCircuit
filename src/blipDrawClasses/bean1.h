/*
 *  bean1.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 26/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef bean1_h
#define bean1_h

#include "baseBlipDraw.h"

class bean1 : public baseBlipDraw {
	
public:
	
	bean1();
	void setup(ofVec2f t_dims);
	void setup(ofVec2f t_dims, baseBlipDraw * t_draw);
	void update();
	void draw(int t_wrap = 0);
	
	vector<ofVec2f> getVertices();
	vector<ofVec2f> getN_verts();
	vector<ofVec2f> getCps_a();
	vector<ofVec2f> getCps_b();
	
	static vector<paramAttributes>getParamDefs();
	
private:
	
	float height, width;
	ofRectangle blankRect;
	vector<ofVec2f> vertices;
	vector<ofVec2f> n_verts;
	vector<ofVec2f> cps_a;
	vector<ofVec2f> cps_b;
	
};


#endif