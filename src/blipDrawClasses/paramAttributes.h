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
	PSET_SLAVE,
	
};

class paramAttributes{
	
	public:
	
	paramAttributes(){
        reset();
	};
	
    void reset(){
        name = "emptyAttribute";
		abs_value = 0;
		min_val = 0;
		max_val = 1;
		setType = PSET_FIXED;
		slaveTo = "";
		modTo = 0;
        modFrom = 0;
        intervals = 0;
    }
    
	string name, slaveTo;
	float abs_value, min_val, max_val;
    float modTo, modFrom;
    int intervals;
	e_setType setType;
};

#endif