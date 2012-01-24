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

enum blipDrawType{

	BT_TESTBLIP,
	BT_ELEC_CURRENT,
	BT_STRAW

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