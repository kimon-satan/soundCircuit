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
#include "utils.h"

class blip : public segment{
	
public:
	
	blip();
	
	void update();
	void draw(int t_wrap = 0);
	
	bool react(float incr);
	
	void createDrawer(ofVec2f t_dims);
	void createDrawer(ofVec2f t_dims, baseBlipDraw * t_d);

	void updateDrawerPosition(ofVec2f t_dims);
	void destroyDrawer();
	void updateDrawer();
	
	bool getInside(ofVec2f t_pos, int bufferZone = 0);
	
	//getters and setters
	bool getIsActive();
	void setIsActive(bool t);
	void addOccupant();
	void subtractOccupant();
	
	void setPreset(blipPreset t);
	void setDuration(float t);
	blipPreset& getPresetRef();
	blipPreset getPreset();
	baseBlipDraw * getDrawer();
	
	void aquireIndex();
	
	static int bCounter;
	
private:
	
	void selectDrawer();
	void updateDrawerParams();
	
	blipPreset preset;
	baseBlipDraw * drawer;
	
	bool isActive;
	int numOccupants;
	
	int attackCount, decayCount, postDecayCount;
	int totalAttack, totalDecay, totalPostDecay;
	float envVal, postVal, duration;
	


};

#endif