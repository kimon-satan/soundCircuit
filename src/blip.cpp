/*
 *  blip.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 10/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "blip.h"


int blip::bCounter = 0;
blipPreset blip::DPRESET = blipPreset();

blip::blip(){

	isActive = false;
	isOccupied = false;
	totalAttack = 0;
	totalDecay = 0;
	attackCount = 0;
	decayCount = 0;
	preset = DPRESET;
	drawer = NULL;

}



void blip::update(){
	
	float attProp, decProp;
	
	if(preset.getEnvType() == ENV_ASR){
		
		if(isOccupied){
			
			if(attackCount > 0){
				attackCount -= 1;
				attProp = 1 - (float)attackCount/(float)totalAttack;
				envVal = attProp;
			}else{envVal =1;}
			
		}else{
			
			if(decayCount == 0){
				isActive = false;
				envVal = 0;
			}
			if(decayCount > 0){
				decayCount -= 1;
				decProp = (float)decayCount/(float)totalDecay;
				envVal = decProp;
			}
		
			
		}
		
	}else if(preset.getEnvType() == ENV_AR){
		
		if(isActive){
			
			if(attackCount > 0){
				attackCount -= 1;
				attProp = 1 - (float)attackCount/(float)totalAttack;
				envVal = attProp;
				
			}else if(decayCount > 0){
				decayCount -= 1;
				decProp = (float)decayCount/(float)totalDecay;
				envVal = decProp;
			
			}else{
				isActive = false;
				envVal =0;
			}
			
		}
	}
	
	updateDrawer();
	
}

void blip::draw(int t_wrap){
	
	if(drawer)drawer->draw(t_wrap);

}

bool blip::react(){
	
	if(!isOccupied){
		isOccupied = true;
		isActive = true;
		attackCount = preset.getAttackSecs().abs_value * ofGetFrameRate();
		decayCount = preset.getDecaySecs().abs_value * ofGetFrameRate();
		totalDecay = decayCount;
		totalAttack = attackCount;
		return true;
	}
	
	return false;
}

void blip::createDrawer(ofVec2f t_dims){
	
	if(preset.getDrawType() == BT_TESTBLIP){drawer = new testBlip();}
	if(preset.getDrawType() == BT_ELEC_CURRENT){drawer = new elecCurrent();}
	if(preset.getDrawType() == BT_STRAW){drawer = new straw();}
	
	drawer->setBlipParams(direction, startPos, endPos,length);
	drawer->setup(t_dims);
	updateDrawer();
	
}

void blip::updateDrawerPosition(ofVec2f t_dims){

	if(drawer)drawer->setup(t_dims);
	
}

void blip::updateDrawer(){

	if(drawer){
		
		drawer->setBlipParams(direction, startPos, endPos,length);
		
		vector<float> t_params;
		
		for(int i =0; i < preset.getVisualParams().size(); i++){
			
			t_params.push_back(preset.getVisualParam(i).abs_value);
		
		}
		
		drawer->setPresetParams(t_params);
		
		drawer->setTimeParams(isActive, envVal);
		
		drawer->update();
	
	}


}


void blip::destroyDrawer(){
	
	if(drawer)delete drawer;
	drawer = NULL;
	
}

//getters and setters

bool blip::getIsActive(){return isActive;}
void blip::setIsActive(bool t){isActive = t;}
void blip::setIsOccupied(bool t){isOccupied = t;}
bool blip::getIsOccupied(){return isOccupied;}
void blip::setPreset(blipPreset t){preset = t;}

blipPreset & blip::getPresetRef(){return preset;}
blipPreset blip::getPreset(){return preset;}

baseBlipDraw * blip::getDrawer(){return drawer;}
