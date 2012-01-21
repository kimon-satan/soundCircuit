/*
 *  blip.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 10/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef blip_h
#define blip_h

#include "segment.h"
#include "blipPreset.h"
#include "blipIncludes.h"


class blip : public segment{
	
public:
	
	blip();
	
	void update();
	void draw(int t_wrap = 0);
	
	bool react();
	
	void createDrawer(ofVec2f t_dims);
	void updateDrawerPosition(ofVec2f t_dims);
	void destroyDrawer();
	void updateDrawer();
	
	//getters and setters
	bool getIsActive();
	void setIsActive(bool t);
	void setIsOccupied(bool t);
	bool getIsOccupied();
	void setPreset(blipPreset t);
	blipPreset& getPresetRef();
	blipPreset getPreset();
	baseBlipDraw * getDrawer();

	
private:
	
	static int bCounter;
	static blipPreset DPRESET;
	blipPreset preset;
	baseBlipDraw * drawer;
	
	bool isActive, isOccupied;
	
	int attackCount, decayCount;
	int totalAttack, totalDecay;
	float envVal;
	


};

#endif