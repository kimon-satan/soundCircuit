//
//  materialiser.h
//  soundcircuit
//
//  Created by Simon Katan on 16/05/2013.
//
//

#ifndef __soundcircuit__materialiser__
#define __soundcircuit__materialiser__

#include "baseBlipDraw.h"

class materialiser : public baseBlipDraw {
	
public:
	
	materialiser();
	void update();
	void draw(int t_wrap = 0);
	static vector<paramAttributes>getParamDefs();
	
private:
	
    void drawRoundedRect(float w, float h , float r);
    ofVec2f genRectPoint(float w, float h , float r);
    
	float alpha;
    float grow;
    ofVec2f wobble;
    float rock;
	ofRectangle testRect;
	bool isDecay;
    

    
   
	
	
};


#endif