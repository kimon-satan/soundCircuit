/*
 *  blipPreset.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 13/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "blipPreset.h"



blipPreset::blipPreset(){

	name = "default";
	synthDef = "basic";
	drawType = BT_TESTBLIP;
	decay_secs.abs_value = 1;
	attack_secs.abs_value = 0.2;
	
	length.abs_value = 100;
	length.setType = PSET_USERA;
	length.min_val = 10;
	length.max_val = 200;
	
	envType = ENV_ASR;
	
	for(int i = 0; i < 5; i++){
		paramAttributes t;
		t.setType = PSET_USERB;
		userParams.push_back(t);
	}
	
	
}


//getters and setters

void blipPreset::setName(string t_name){name = t_name;}
string blipPreset::getName()const{return name;}

void blipPreset::setEnvType(e_envType t_env){envType = t_env;}
e_envType blipPreset::getEnvType()const{return envType;}

void blipPreset::setSynthDef(string t_def){synthDef = t_def;}
string blipPreset::getSynthDef(){return synthDef;}

blipDrawType blipPreset::getDrawType(){return drawType;}
void blipPreset::setDrawType(blipDrawType t){drawType = t;}

paramAttributes& blipPreset::getUserParam(int p){return userParams[p];}
paramAttributes& blipPreset::getAttackSecs(){return attack_secs;}
paramAttributes& blipPreset::getDecaySecs(){return decay_secs;}
paramAttributes& blipPreset::getLength(){return length;}
vector<paramAttributes>& blipPreset::getUserParams(){return userParams;}

