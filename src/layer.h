/*
 *  layer.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 04/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef layer_h
#define layer_h

#include "ofMain.h"
#include "objectRenderer.h"
#include "reader.h"

enum e_objectType{
	OT_WORLD,
	OT_NODE,
	OT_TRACK,
	OT_BLIP,
	OT_READER,
	OT_COUNT
};


struct readerIndex:public binary_function<reader, int, bool> {
	
	bool operator()(const reader &t_reader, const int &t_index)const{
		return t_reader.getIndex() == t_index;
	}
	
};

class layer : public objectRenderer{

public:
	

	
	layer();
	//layer(const layer &src); only one layer so no need currently
	
	void setup();
	void update();
	void draw(ofVec2f pos, ofRectangle roi, ofColor col);
	
	e_objectType selectSomething(ofVec2f w_pos, bool isReader = true, bool isBlip = true, bool isNode = true, bool isTrack = true);
	void deselectAll();
	void deselectReaders();
	
	void beginInsertion(ofVec2f t_pos, ofVec2f t_dir);
	void resizeInsertion(float size);
	void endInsertion();

	reader * addReader(ofVec2f w_pos);
	reader * selectReader(ofVec2f w_pos);
	reader * getNearestReader(ofVec2f w_pos);
	void destroyReader(reader * r_ptr);
	

	
	void setOSC(ofxOscSender * s);
	
	//getters and setters

	vector<node> * getNodesRef();
	vector<blip> * getBlipsRef();
	
	vector<node> getNodes()const;
	vector<blip> getBlips()const;
	vector<segment> getTracks() const;
	
	void toggleScreenData();
	reader * getReaderRef(int i = 0);
	reader getReader(int i = 0);
	
	
private:
	
	vector<reader> mReaders;
	bool isScreenData;
	ofxOscSender * sender;
	reader * selectedReader;
	ofColor testCol;
	
};

#endif