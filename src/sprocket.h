//
//  sprocket.h
//  soundcircuit
//
//  Created by Simon Katan on 20/05/2013.
//
//

#ifndef __soundcircuit__sprocket__
#define __soundcircuit__sprocket__

#include "baseBlipDraw.h"

class sprocket : public baseBlipDraw {
	
public:
	
	sprocket();
	void update();
	void draw(int t_wrap = 0);
	static vector<paramAttributes>getParamDefs();
	
private:
	
    void drawSprocket(float w, float h , float r);
    
	float alpha;
    ofColor c;
 
	ofRectangle testRect;
	bool isDecay;
    
    
	
};


#endif /* defined(__soundcircuit__sprocket__) */

