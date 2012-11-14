/*
 *  bank.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 22/03/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BANK_H
#define BANK_H

#include "blipPreset.h"

class  bank{
	
public:
	
	bank();	
	string getSelectedName();
	blipPreset getSelectedPreset();
	blipPreset & getSelectedRef();
	void incrementPreset();
	void decrementPreset();
	
	string name;
	vector<vector<blipPreset> > presets;
	int selectedPreset[2];

	
};

#endif