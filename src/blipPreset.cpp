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
	decay_secs.abs_value = 1;
	attack_secs.abs_value = 0.2;
	
	length.abs_value = 100;
	length.setType = PSET_USERA;
	length.min_val = 10;
	length.max_val = 300;
	
	envType = ENV_ASR;
	
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

paramAttributes& blipPreset::getSoundParam(int p){return soundParams[p];}
paramAttributes& blipPreset::getSoundParam(string s){
	
	for(int i = 0; i < soundParams.size(); i ++){
		if(s == soundParams[i].name)return soundParams[i];
	}
	
	return soundParams[0]; //probably need a better solution to this
}

paramAttributes& blipPreset::getVisualParam(int p){return visualParams[p];}
paramAttributes& blipPreset::getVisualParam(string s){
	
	for(int i = 0; i < visualParams.size(); i ++){
		if(s == visualParams[i].name)return visualParams[i];
	}
	
	return visualParams[0]; //probably need a better solution to this
}

paramAttributes& blipPreset::getAttackSecs(){return attack_secs;}
paramAttributes& blipPreset::getDecaySecs(){return decay_secs;}
paramAttributes& blipPreset::getPostDecaySecs(){return post_decay;}
paramAttributes& blipPreset::getLength(){return length;}
vector<paramAttributes>& blipPreset::getSoundParams(){return soundParams;}
vector<paramAttributes>& blipPreset::getVisualParams(){return visualParams;}

void blipPreset::setUserVals(float a, float b){userVals[0] = a; userVals[1] = b;}
vector<float> blipPreset::getUserVals(){return userVals;}
