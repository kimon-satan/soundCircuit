/*
 *  bank.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 22/03/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "bank.h"

bank::bank(){
	
	selectedPreset[0] = 0;
	selectedPreset[1] = 0;
}


string bank::getSelectedName(){

	return presets[selectedPreset[0]][selectedPreset[1]].getName();

}


blipPreset bank::getSelectedPreset(){

	return presets[selectedPreset[0]][selectedPreset[1]];
	
}

blipPreset & bank::getSelectedRef(){
	
	return presets[selectedPreset[0]][selectedPreset[1]];
	
}

void bank::incrementPreset(){

	selectedPreset[0] = min(selectedPreset[0] + 1, (int)presets.size() - 1);

}

void bank::decrementPreset(){

	selectedPreset[0] = max(selectedPreset[0] - 1, 0);
}
