/*
 *  objectManager.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 04/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef objectManager_h
#define objectManager_h

#include "objectUtils.h"
#include "node.h"
#include "blip.h"

class objectManager : public objectUtils{
	
public:

	objectManager();
	
	//accessible functions
	
	void beginTrack(ofVec2f w_pos);
	void calcTrack(ofVec2f w_pos, ofVec2f t_dir, int mode);
	void endTrack();
	
	void beginBlip(ofVec2f w_pos, blipPreset bp);
	void calcBlip(ofVec2f w_pos, ofVec2f dir);
	void endBlip();
	
	void setParam(paramAttributes * p, float userA, float userB, float m_val);
	string getPreviewParams();
	
	//getters and setters
	
	void setTracks(vector<segment> * t_ref);
	void setNodes(vector<node> * t_ref);
	void setBlips(vector<blip> * t_ref);
	
	void deselectNodes();
	void deselectTracks();
	node * selectNode(ofVec2f t_pos);
	segment * selectTrackPoint(ofVec2f t_pos);
	segment * selectBlip(ofVec2f t_pos);
	
	
protected:
	
	//construction
	
	void calcTrack_0(ofVec2f w_pos, ofVec2f dir);
	void calcTrack_1(ofVec2f w_pos, ofVec2f dir);
	
	void constrainFromMidPoint(ofVec2f origin, segment & s, bool isTracks = false, bool isNodes = true, bool isBlips = false);
	void constrainEndPoint(ofVec2f origin, segment & s, bool isTracks = true, bool isNodes = false, bool isBlips = false);
	void repositionFromMidPoint(ofVec2f origin, segment & s, bool isTracks = false, bool isNodes = true, bool isBlips = false);
	
	//searches
	
	bool findCrossIntersects(ofVec2f origin, ofVec2f t_dir, vector<ofVec2f> & t_points = DPOINTS, segment & s = DSEG, bool isTracks = true , bool isBlips = false);
	bool findParalellIntersects(segment & s = DSEG, vector<ofVec2f> & t_points = DPOINTS, bool isTracks = true , bool isBlips = false);
	bool findNodeIntersects(segment & s, vector<ofVec2f> & t_points = DPOINTS);
	
	
	//member variables
	
	static vector<ofVec2f> DPOINTS;
	static segment DSEG;
	
	vector <segment> * tracks;
	vector <node> * nodes;
	vector <blip> * blips;
	
	segment previewTracks[2];
	blip previewBlip;
	
	node * s_nodes[2];
	segment * s_tracks[2];
	ofVec2f s_pos[2];
	
	bool isPreview;
	
};

#endif