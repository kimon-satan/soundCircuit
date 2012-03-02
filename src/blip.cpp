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

blip::blip(){

	isActive = false;
	isOccupied = false;
	totalAttack = 0;
	totalDecay = 0;
	attackCount = 0;
	decayCount = 0;
	postDecayCount =0;
	envVal = 0;
	postVal = 0;
	
	drawer = NULL;

}



void blip::update(){
	
	
	if(preset.getEnvType() == ENV_ASR){
		
		if(isOccupied){
			
			postVal = 0;
			
			if(attackCount > 0){
				attackCount -= 1;
				envVal = 1 - (float)attackCount/(float)totalAttack;
			}else{envVal =1;}
			
		}else{
			
			if(decayCount > 0){
				if(postDecayCount > 0)postVal = 1;
				decayCount -= 1;
				envVal = (float)decayCount/(float)totalDecay;
				
			}else{
				
				envVal = 0;
				
				if(postDecayCount > 0 ){
					postDecayCount -= 1;
					postVal = (float)postDecayCount/(float)totalPostDecay;
				}else{
					postVal = 0;
					isActive = false;
				}
				
			}

		
			
		}
		
	}else if(preset.getEnvType() == ENV_AR){
		
		if(isActive){
	
			if(attackCount > 0){
				attackCount -= 1;
				envVal = 1 - (float)attackCount/(float)totalAttack;
				
			}else if(decayCount > 0){
				
				if(postDecayCount > 0)postVal = 1;
				decayCount -= 1;
				envVal = (float)decayCount/(float)totalDecay;
			
			}else{
				
				envVal = 0;
				
				
				if(postDecayCount > 0 ){
					postDecayCount -= 1;
					postVal = (float)postDecayCount/(float)totalPostDecay;
					
				}else{
					postVal = 0;
					isActive = false;
				}
				
			}
			
		}
	}
	
	updateDrawer();
	
}

void blip::draw(int t_wrap){
	
	if(drawer)drawer->draw(t_wrap);

}

bool blip::react(float incr){
	
	if(!isOccupied){
		isOccupied = true;
		isActive = true;
		
		paramAttributes * p = preset.getAttackProp();
		if(p)preset.getAttackSecs()->abs_value = (p->abs_value * length)/(incr * ofGetFrameRate());
		p = preset.getDecayProp();
		if(p)preset.getDecaySecs()->abs_value = (p->abs_value * length)/(incr * ofGetFrameRate());
		p = preset.getPostDecayProp();
		if(p)preset.getPostDecaySecs()->abs_value = (p->abs_value * length)/(incr * ofGetFrameRate());
		
		attackCount = preset.getAttackSecs()->abs_value * ofGetFrameRate();
		decayCount = preset.getDecaySecs()->abs_value * ofGetFrameRate();
		postDecayCount = preset.getPostDecaySecs()->abs_value * ofGetFrameRate();
		totalPostDecay = postDecayCount;
		totalDecay = decayCount;
		totalAttack = attackCount;
		return true;
	}
	
	return false;
}


void blip::createDrawer(ofVec2f t_dims, baseBlipDraw * t_draw){
	
	selectDrawer();
	updateDrawerParams();
	drawer->setup(t_dims, t_draw);
	drawer->setDecayRatio(preset.getDecaySecs()->abs_value , preset.getPostDecaySecs()->abs_value);
	updateDrawer();
}

void blip::createDrawer(ofVec2f t_dims){

	selectDrawer();
	drawer->setBlipParams(direction, startPos, endPos,length);
	updateDrawerParams();
	drawer->setup(t_dims);
	updateDrawer();
	
}

void blip::selectDrawer(){
	
	if(preset.getDrawType() == BT_TESTBLIP){drawer = new testBlip();}
	if(preset.getDrawType() == BT_ELEC_CURRENT){drawer = new elecCurrent();}
	if(preset.getDrawType() == BT_STRAW){drawer = new straw();}
	if(preset.getDrawType() == BT_BELCH){drawer = new belch();}
	if(preset.getDrawType() == BT_BEAN1){drawer = new bean1();}
	if(preset.getDrawType() == BT_FLIPPER){drawer = new flipper();}

}

void blip::updateDrawerPosition(ofVec2f t_dims){

	if(drawer)drawer->setup(t_dims);
	
}

void blip::updateDrawerParams(){

	vector<float> t_params;
	for(int i =0; i < preset.getVisualParams()->size(); i++)t_params.push_back(preset.getVisualParam(i)->abs_value);
	drawer->setPresetParams(t_params);

}

void blip::updateDrawer(){

	if(drawer){
		
		drawer->setBlipParams(direction, startPos, endPos,length);
		updateDrawerParams();
		drawer->setTimeParams(isActive, envVal, postVal);
		drawer->update();
	
	}


}


void blip::destroyDrawer(){
	
	if(drawer)delete drawer;
	drawer = NULL;
	
}


bool blip::getInside(ofVec2f t_pos, int bufferZone){
	

		
		ofRectangle n_testArea;
		ofRectangle n_wrapTestArea;
		
		n_testArea.setFromCenter(testArea.getCenter(), 
								 testArea.width + bufferZone * 2,
								 testArea.height + bufferZone * 2);
	
		
		if(!isWrapped){
	
			return (n_testArea.inside(t_pos));
		}else{
			
			n_wrapTestArea.setFromCenter(wrapTestArea.getCenter(), 
									 wrapTestArea.width + bufferZone * 2,
									 wrapTestArea.height + bufferZone * 2);
			
			if(n_testArea.inside(t_pos))return true;
			if(n_wrapTestArea.inside(t_pos))return true;
			return false;
		}
	
	
	
	

}

//getters and setters

bool blip::getIsActive(){return isActive;}
void blip::setIsActive(bool t){isActive = t;}
void blip::setIsOccupied(bool t){isOccupied = t;}
bool blip::getIsOccupied(){return isOccupied;}
void blip::setPreset(blipPreset t){preset = t;}

void blip::setDuration(float t){duration = t;}

blipPreset & blip::getPresetRef(){return preset;}
blipPreset blip::getPreset(){return preset;}

baseBlipDraw * blip::getDrawer(){return drawer;}
