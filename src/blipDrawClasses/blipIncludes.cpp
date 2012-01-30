/*
 *  blipIncludes.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 19/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "blipIncludes.h"


vector<paramAttributes> drawDictionary::getDefinition(blipDrawType drawDef){
	
	vector<paramAttributes> blank;
	
	switch (drawDef) {
		case BT_TESTBLIP:return testBlip::getParamDefs();break;
		case BT_ELEC_CURRENT:return elecCurrent::getParamDefs();break;
		case BT_STRAW:return straw::getParamDefs();break;
		case BT_BELCH:return belch::getParamDefs();break;
		case BT_BEAN1:return bean1::getParamDefs();break;
	}

	return blank;
}

