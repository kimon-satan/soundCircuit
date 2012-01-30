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

enum blipDrawType{

	BT_TESTBLIP,
	BT_ELEC_CURRENT,
	BT_STRAW,
	BT_BELCH,
	BT_BEAN1

};


class drawDictionary{
	
public:
	
	static vector<paramAttributes> getDefinition(blipDrawType drawDef);
		

	
};


#endif