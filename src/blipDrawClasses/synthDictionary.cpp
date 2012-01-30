/*
 *  synthDictionary.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 30/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "synthDictionary.h"

synthDictionary::synthDictionary(){

	
}

void synthDictionary::loadDictionary(){

	ofxXmlSettings XML;
	
	if(XML.loadFile("synthDefs.xml")){
		
		if(XML.pushTag("SOUND_CIRCUIT", 0)){
			
			if(XML.pushTag("SYNTH_DEFS", 0)){
				
				int numDefs = XML.getNumTags("DEF");
				
				for(int i = 0; i < numDefs; i ++){
				
					if(XML.pushTag("DEF", i)){
						
						string name  = XML.getValue("NAME", "");
						vector<paramAttributes> params;
						int numParams = XML.getNumTags("PARAM");
						
						for(int j = 0; j < numParams; j++){
								
							if(XML.pushTag("PARAM", j)){
							
								paramAttributes p;
								p.name = XML.getValue("T", "");
								params.push_back(p);
								XML.popTag();
							}
							
						}
						
						dictionary[name] = params;
						
						XML.popTag();
					}
				
				}
			
				XML.popTag();
			}
			
			XML.popTag();
		}
	
	
	}

}

vector<paramAttributes> synthDictionary::getDefinition(string synthDef){
	
	return dictionary[synthDef];
	
}