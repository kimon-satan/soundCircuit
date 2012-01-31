/*
 *  blipPreset.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 13/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef blipPreset_h
#define blipPreset_h

#include "blipIncludes.h"
#include "synthDictionary.h"

enum e_envType {
	ENV_ASR,
	ENV_AR
};


class blipPreset {
	
	
public:
	
	blipPreset();
	
	//getters and setters

	void setName(string t_name);
	string getName()const;
	void setEnvType(e_envType t_env);
	e_envType getEnvType()const;
	void setSynthDef(string t_def);
	string getSynthDef();
	blipDrawType getDrawType();
	void setDrawType(blipDrawType t);
	void setDrawType(string s);
	
	paramAttributes * getAttackSecs();
	paramAttributes * getDecaySecs();
	paramAttributes * getPostDecaySecs();
	paramAttributes * getAttackProp();
	paramAttributes * getDecayProp();
	paramAttributes * getPostDecayProp();
	paramAttributes * getLength();
	
	paramAttributes * getSoundParam(int p);
	paramAttributes * getSoundParam(string s);
	vector<paramAttributes> * getSoundParams();
	paramAttributes * getVisualParam(int p);
	paramAttributes * getVisualParam(string s);
	vector<paramAttributes> * getVisualParams();
	
	void setUserVals(float a, float b);
	vector<float> getUserVals();
	
	bool getIsRandSet();
	void setIsRandSet(bool t);
	
	void setIsAttackProp(bool t);
	void setIsDecayProp(bool t);
	void setIsPostDecayProp(bool t);
	
	static synthDictionary thisSynthDef;
	
private:
	
	
	string name;
	string synthDef;
	blipDrawType drawType;
	bool isRandsSet;
	bool isAttProp, isDecProp, isPostDecProp;
	
	vector<float> userVals;
	
	e_envType envType;
	paramAttributes attack[2], decay[2], postDecay[2], length;
	vector<paramAttributes> soundParams;
	vector<paramAttributes> visualParams;
	

	
};

#endif