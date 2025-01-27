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
	totalAttack = 0;
	totalDecay = 0;
	attackCount = 0;
	decayCount = 0;
	postDecayCount =0;
	envVal = 0;
	postVal = 0;
	numOccupants = 0;
    nTriggered = 0;
	
	drawer = NULL;

}



void blip::update(){
	
	
	if(preset.getEnvType() == ENV_ASR){
		
		if(numOccupants > 0){
			
			postVal = 0;
			
			if(attackCount > 0){
				attackCount -= 1;
				envVal = 1 - (float)attackCount/(float)totalAttack;
			}else{
                envVal =1;
            }
			
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

void blip::prereact(float incr){

    //set for proportional attacks and decays
    paramAttributes * p = preset.getAttackProp();
	if(p)preset.getAttackSecs()->abs_value = (p->abs_value * length)/(incr * ofGetFrameRate());
	p = preset.getDecayProp();
	if(p)preset.getDecaySecs()->abs_value = (p->abs_value * length)/(incr * ofGetFrameRate());
	p = preset.getPostDecayProp();
	if(p)preset.getPostDecaySecs()->abs_value = (p->abs_value * length)/(incr * ofGetFrameRate());
    
}


bool blip::react(){

    
    updateModParams(); //for persistent attributes 
	isActive = true;
	
	attackCount = preset.getAttackSecs()->abs_value * ofGetFrameRate();
	decayCount = preset.getDecaySecs()->abs_value * ofGetFrameRate();
	postDecayCount = preset.getPostDecaySecs()->abs_value * ofGetFrameRate();
	totalPostDecay = postDecayCount;
	totalDecay = decayCount;
	totalAttack = attackCount;
	return true;


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
    if(preset.getDrawType() == BT_SLIDER){drawer = new slider();}
    if(preset.getDrawType() == BT_MATERIALISER){drawer = new materialiser();}
    if(preset.getDrawType() == BT_SPROCKET){drawer = new sprocket();}
    if(preset.getDrawType() == BT_RATCHET){drawer = new ratchet();}

}

void blip::updateDrawerPosition(ofVec2f t_dims){

	if(drawer)drawer->setup(t_dims);
	
}

void blip::updateDrawerParams(){

    vector<float> t_params;
    for(int i =0; i < preset.getVisualParams()->size(); i++){
        
        t_params.push_back(preset.getVisualParam(i)->abs_value);
        
    }
    
    drawer->setPresetParams(t_params);

}

void blip::updateDrawer(){

	if(drawer){
		drawer->setBlipParams(direction, startPos, endPos,length);
		if(index == -99)updateDrawerParams();
		drawer->setTimeParams(isActive, envVal, postVal);
		drawer->update();
	
	}


}

void blip::updateModParams(){

    for(int i = 0; i < preset.getSoundParams()->size(); i ++){
        
        paramAttributes * p = preset.getSoundParam(i);
        
        if(p->modTo > 0)modParam(p);
    
    }
    
    for(int i = 0; i < preset.getVisualParams()->size(); i ++){
        
        paramAttributes * p = preset.getVisualParam(i);
        
        if(p->modTo > 0)modParam(p);
        
    }
    
    updateDrawerParams();


}


void blip::modParam(paramAttributes *p){

    float r = p->modTo - p->modFrom;
    float add = r/(p->intervals - 1);
    float nv =  p->abs_value - p->modFrom + add;
    nv = fmod(nv, add * p->intervals) + p->modFrom;
    p->abs_value = nv;
    
}

void blip::destroyDrawer(){
	
	if(drawer)delete drawer;
	drawer = NULL;
	
}


bool blip::getInside(ofVec2f t_pos, int bufferZone){
	
	ofRectangle n_testArea;
	ofRectangle n_wrapTestArea;
	
	n_testArea.setFromCenter(testArea.getCenter(), 
							 testArea.width + (float)bufferZone * WORLD_UNIT * 2,
							 testArea.height + (float)bufferZone * WORLD_UNIT * 2);

	
	if(!isWrapped){

		return (n_testArea.inside(t_pos));
	}else{
		
		n_wrapTestArea.setFromCenter(wrapTestArea.getCenter(), 
								 wrapTestArea.width + bufferZone * WORLD_UNIT * 2,
								 wrapTestArea.height + bufferZone * WORLD_UNIT * 2);
		
		if(n_testArea.inside(t_pos))return true;
		if(n_wrapTestArea.inside(t_pos))return true;
		return false;
	}
	

}


void blip::aquireIndex(){
	
	index = bCounter;
	bCounter += 1;
	
}

//getters and setters
void blip::addOccupant(){numOccupants += 1;}
void blip::subtractOccupant(){numOccupants -= 1;}

bool blip::getIsActive(){return isActive;}
void blip::setIsActive(bool t){isActive = t;}

void blip::setPreset(blipPreset t){preset = t;}

void blip::setDuration(float t){duration = t;}

blipPreset & blip::getPresetRef(){return preset;}
blipPreset blip::getPreset(){return preset;}

baseBlipDraw * blip::getDrawer(){return drawer;}

int blip::getNTriggered(){return nTriggered;}
