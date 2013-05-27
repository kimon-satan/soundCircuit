

#ifndef __soundcircuit__ratchet__
#define __soundcircuit__ratchet__

#include "baseBlipDraw.h"

class ratchet : public baseBlipDraw {
	
public:
	
	ratchet();
	void update();
	void draw(int t_wrap = 0);
	static vector<paramAttributes>getParamDefs();
	
private:
	
	float slide;
    float c_pos;
    int sl_mul;
    
	ofColor c;
	ofRectangle testRect;
	bool isDecay, isPost;
    int alpha;
	
	
};


#endif /* defined(__soundcircuit__ratchet__) */
