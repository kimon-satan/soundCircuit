//
//  slider.h
//  soundcircuit
//
//  Created by Simon Katan on 13/05/2013.
//
//

#ifndef __soundcircuit__slider__
#define __soundcircuit__slider__

#include "baseBlipDraw.h"

class slider : public baseBlipDraw {
	
public:
	
	slider();
	void update();
	void draw(int t_wrap = 0);
	static vector<paramAttributes>getParamDefs();
	
private:
	
	float slide;
    float c_pos;
    float wobble;

    ofVec2f wobblePos;
	ofColor c;
	ofRectangle testRect;
	bool isDecay;
	
	
};


#endif /* defined(__soundcircuit__slider__) */
