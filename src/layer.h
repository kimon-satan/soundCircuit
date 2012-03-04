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

class layer : public objectRenderer{

public:
	
	layer();
	layer(const layer &src);
	
	void setup();
	void update();
	void draw();

	void draw(ofRectangle& vp, bool isDummy = false);
	void drawPreview(ofRectangle vp);
	
	void selectSomething(ofVec2f w_pos);
	void deselectAll();
	
	void beginInsertion(ofVec2f t_pos, ofVec2f t_dir);
	void resizeInsertion(float size);
	void endInsertion();
	
	void rotate(int plane);
	
	//getters and setters

	vector<node> * getNodesRef();
	vector<blip> * getBlipsRef();
	
	vector<node> getNodes()const;
	vector<blip> getBlips()const;
	vector<segment> getTracks() const;
	
	void toggleScreenData();
	reader * getReaderRef();
	reader getReader()const;
	
	vector<float> getTrans()const;
	vector<float> getRots()const;
	vector<float> getTargetTrans() const;
	vector<float> getTargetRots() const;

	float getRot(int i);
	void setRot(int i, float t_rot);
	void setTrans(int i, float t_trans);

private:
	
	reader mReader;
	bool isScreenData;
	vector<float>trans;
	vector<float>rots;
	
	vector<float>targetTrans;
	vector<float>targetRots;
	
};

#endif