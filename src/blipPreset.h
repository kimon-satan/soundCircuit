/*
 *  blipPreset.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 13/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef blipPreset_h
#define blipPreset_h

#include "ofMain.h"
#include "blipIncludes.h"

enum e_envType {
	ENV_ASR,
	ENV_AR
};

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


class blipPreset {
	
	
public:
	
	blipPreset();
	
	//getters and setters

	void setName(string t_name);
	string getName()const;
	void setEnvType(e_envType t_env);
	e_envType getEnvType()const;
	void setSynthDef(string t_def);
	string getSynthDef();
	blipDrawType getDrawType();
	void setDrawType(blipDrawType t);
	
	paramAttributes& getUserParam(int p);
	paramAttributes& getAttackSecs();
	paramAttributes& getDecaySecs();
	paramAttributes& getLength();
	
	vector<paramAttributes>& getUserParams();
	

private:
	
	string name;
	string synthDef;
	blipDrawType drawType;
	
	e_envType envType;
	paramAttributes attack_secs, decay_secs, length;
	vector<paramAttributes> userParams;
	

	
};

#endif