/*
 *  paramAttributes.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 30/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef paramAttributes_h
#define paramAttributes_h

enum e_setType{
	
	PSET_FIXED,
	PSET_USERA,
	PSET_USERB,
	PSET_MAP,
	PSET_RANDOM,
	PSET_SLAVE
	
};

class paramAttributes{
	
	public:
	
	paramAttributes(){
		name = "emptyAttribute";
		abs_value = 0;
		min_val = 0;
		max_val = 1;
		setType = PSET_FIXED;
		slaveTo = "";
		
	};
	
	string name, slaveTo;
	float abs_value, min_val, max_val;
	e_setType setType;
};

#endif