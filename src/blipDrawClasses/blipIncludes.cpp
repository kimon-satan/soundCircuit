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
		case BT_FLIPPER:return flipper::getParamDefs();break;
        case BT_SLIDER:return slider::getParamDefs();break;
        case BT_MATERIALISER:return materialiser::getParamDefs();break;
        case BT_SPROCKET: return sprocket::getParamDefs();break;
        case BT_RATCHET: return ratchet::getParamDefs();break;
	}

	return blank;
}

blipDrawType drawDictionary::getTypeFromString(string s){

	if(s == "testBlip"){return BT_TESTBLIP;}
	if(s == "elecCurrent"){return BT_ELEC_CURRENT;}
	if(s == "straw"){return BT_STRAW;}
	if(s == "belch"){return BT_BELCH;}
	if(s == "bean1"){return BT_BEAN1;}
	if(s == "flipper"){return BT_FLIPPER;}
    if(s == "slider"){return BT_SLIDER;}
    if(s == "materialiser"){return BT_MATERIALISER;}
    if(s == "sprocket"){return BT_SPROCKET;}
    if(s == "ratchet"){return BT_RATCHET;}
	
	return BT_TESTBLIP;
	
}