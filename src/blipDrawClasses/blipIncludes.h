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
#include "flipper.h"
#include "slider.h"
#include "materialiser.h"
#include "sprocket.h"

enum blipDrawType{

	BT_TESTBLIP,
	BT_ELEC_CURRENT,
	BT_STRAW,
	BT_BELCH,
	BT_BEAN1,
	BT_FLIPPER,
    BT_SLIDER,
    BT_MATERIALISER,
    BT_SPROCKET

};


class drawDictionary{
	
public:
	
	static vector<paramAttributes> getDefinition(blipDrawType drawDef);
	static blipDrawType getTypeFromString(string s);
		

	
};


#endif