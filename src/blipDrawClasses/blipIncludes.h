/*
 *  blipIncludes.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 13/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef blipIncludes_h
#define blipIncludes_h

#include "testBlip.h"
#include "elecCurrent.h"
#include "straw.h"
#include "belch.h"
#include "bean1.h"

enum e_setType{
	
	PSET_FIXED,
	PSET_USERA,
	PSET_USERB,
	PSET_MAP,
	PSET_RANDOM,
	PSET_SLAVE
	
};

struct paramAttributes{
	
	paramAttributes(){
		name = "emptyAttribute";
		abs_value = 0;
		min_val = 0;
		max_val = 1;
		setType = PSET_FIXED;
		slaveTo = "";
	}
	
	string name, slaveTo;
	float abs_value, min_val, max_val;
	e_setType setType;
};


enum blipDrawType{

	BT_TESTBLIP,
	BT_ELEC_CURRENT,
	BT_STRAW,
	BT_BELCH,
	BT_BEAN1

};

class drawDictionary{
	
public:
	
	drawDictionary();
	vector<paramAttributes> getDefinition(blipDrawType drawDef);
	
private:
	
	paramAttributes hue;
	paramAttributes rotation;
	paramAttributes pivot;
	paramAttributes density;
	paramAttributes height;
	paramAttributes speed;
	paramAttributes thickness;

	
};

class synthDictionary{
	
public:
	
	synthDictionary();
	vector<paramAttributes> getDefinition(string synthDef);
	
private:
	
	paramAttributes freq, amp, clip, speed, separate, attack; //this potentially could be done with an XML file
	

};

#endif