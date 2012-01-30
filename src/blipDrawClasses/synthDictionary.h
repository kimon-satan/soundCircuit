/*
 *  synthDictionary.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 30/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef synthDictionary_h
#define synthDictionary_h

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "paramAttributes.h"

class synthDictionary{
	
public:
	
	synthDictionary();
	void loadDictionary();
	vector<paramAttributes> getDefinition(string synthDef);
	
private:

	map<string, vector<paramAttributes> > dictionary;
	
	
};


#endif