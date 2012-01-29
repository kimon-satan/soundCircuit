/*
 *  blipPreset.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 13/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "blipPreset.h"

synthDictionary blipPreset::thisSynthDef = synthDictionary();
drawDictionary blipPreset::thisDrawDef = drawDictionary();

blipPreset::blipPreset(){

	name = "default";
	synthDef = "basic";
	drawType = BT_TESTBLIP;
	decay[0].abs_value = 1;
	attack[0].abs_value = 0.2;
	
	length.abs_value = 100;
	length.setType = PSET_USERA;
	length.min_val = 10;
	length.max_val = 300;
	
	envType = ENV_ASR;
	isRandsSet = false;
	
	isAttProp = false; isDecProp = false; isPostDecProp = false;
	
	userVals.push_back(0);
	userVals.push_back(0);
	
	
}


//getters and setters

void blipPreset::setName(string t_name){name = t_name;}
string blipPreset::getName()const{return name;}

void blipPreset::setEnvType(e_envType t_env){envType = t_env;}
e_envType blipPreset::getEnvType()const{return envType;}

void blipPreset::setSynthDef(string t_def){
	synthDef = t_def;
	soundParams.clear();
	soundParams = thisSynthDef.getDefinition(synthDef);
}
string blipPreset::getSynthDef(){return synthDef;}

blipDrawType blipPreset::getDrawType(){return drawType;}
void blipPreset::setDrawType(blipDrawType t){
	drawType = t;
	visualParams.clear();
	visualParams = thisDrawDef.getDefinition(drawType);
}

paramAttributes * blipPreset::getSoundParam(int p){return &soundParams[p];}
paramAttributes * blipPreset::getSoundParam(string s){
	
	for(int i = 0; i < soundParams.size(); i ++){
		if(s == soundParams[i].name)return &soundParams[i];
	}
	
	return NULL; 
}

paramAttributes * blipPreset::getVisualParam(int p){return &visualParams[p];}
paramAttributes * blipPreset::getVisualParam(string s){
	
	for(int i = 0; i < visualParams.size(); i ++){
		if(s == visualParams[i].name)return &visualParams[i];
	}
	
	return NULL;
}

paramAttributes * blipPreset::getAttackSecs(){return &attack[0];}
paramAttributes * blipPreset::getDecaySecs(){return &decay[0];}
paramAttributes * blipPreset::getPostDecaySecs(){return &postDecay[0];}

paramAttributes * blipPreset::getAttackProp(){if(isAttProp){return &attack[1];}else{return NULL;} }
paramAttributes * blipPreset::getDecayProp(){if(isDecProp){return &decay[1];}else{return NULL;}}
paramAttributes * blipPreset::getPostDecayProp(){if(isPostDecProp){return &postDecay[1];}else{return NULL;}}

paramAttributes * blipPreset::getLength(){return &length;}
vector<paramAttributes> * blipPreset::getSoundParams(){return &soundParams;}
vector<paramAttributes> * blipPreset::getVisualParams(){return &visualParams;}

void blipPreset::setUserVals(float a, float b){userVals[0] = a; userVals[1] = b;}
vector<float> blipPreset::getUserVals(){return userVals;}
bool blipPreset::getIsRandSet(){return isRandsSet;}
void blipPreset::setIsRandSet(bool t){isRandsSet = t;}

void blipPreset::setIsAttackProp(bool t){isAttProp  = t;}
void blipPreset::setIsDecayProp(bool t){isDecProp = t;}
void blipPreset::setIsPostDecayProp(bool t){isPostDecProp = t;}
