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
#include "reader.h"

class objectManager : public objectUtils{
	
public:

	objectManager();
	
	//accessible functions
	
	void beginTrack(ofVec2f w_pos);
	void calcTrack(ofVec2f w_pos, ofVec2f w_dir, int mode);
	void endTrack();
	
	void beginBlip(ofVec2f w_pos, blipPreset bp);
	void calcBlip(ofVec2f w_pos, ofVec2f dir, float s_angle);
	void endBlip();
	
	void beginNode(ofVec2f w_pos, int mode);
	void calcNode(ofVec2f w_pos);
	void endNode();
	
	void setParam(paramAttributes * p, float userA, float userB, float m_val);
	vector<string> getPreviewParams();
	
	virtual void beginInsertion(ofVec2f t_point, ofVec2f t_dir);
	virtual void resizeInsertion(float size);
	virtual void endInsertion();
	
	void beginInsertSpace(ofVec2f t_point, ofVec2f t_dir, float t_size);
	segment splitSegment(segment &s, ofVec2f s_point, float splitRadius);
	void splitBlip(blip & b, ofVec2f t_point);
	void splitTrack(segment & s, ofVec2f t_point);
	
	void destroyBlip(blip & tb);
	void destroyTrack(segment & ts);

	
	//getters and setters

	
	void deselectNodes();
	void deselectTracks();
	node * selectNode(ofVec2f t_pos, int rad = 10);
	segment * selectTrackPoint(ofVec2f t_pos);
	blip * selectBlip(ofVec2f t_pos, int bZone = 0);
	

	
	
protected:
	
	//construction
	
	void calcTrack_0(ofVec2f w_pos, ofVec2f dir);
	void calcTrack_1(ofVec2f w_pos, ofVec2f dir);
	
	void addNodesToTrack(segment & s);
	
	void constrainFromMidPoint(ofVec2f origin, segment & s, bool isTracks = false, bool isNodes = true, bool isBlips = false);
	void constrainEndPoint(ofVec2f origin, segment & s, bool isTracks = true, bool isNodes = false, bool isBlips = false);
	void repositionFromMidPoint(ofVec2f origin, segment & s, bool isTracks = false, bool isNodes = true, bool isBlips = false);
	
	
	//searches
	
	bool findCrossIntersects(ofVec2f origin, ofVec2f t_dir, vector<ofVec2f> & t_points = DPOINTS, segment & s = DSEG, bool isTracks = true , bool isBlips = false);
	bool findParalellIntersects(segment & s = DSEG, vector<ofVec2f> & t_points = DPOINTS, bool isTracks = true , bool isBlips = false);
	bool findNodeIntersects(segment & s, vector<ofVec2f> & t_points = DPOINTS, bool includeTestArea = false);
	
	
	//helpers
	
	void updateBlipDraw(blip & tb);
	ofVec2f flipBoundaryPoint(ofVec2f t);
	bool checkNode(node & n, ofVec2f t_pos, int rad);
	
	
	//member variables
	
	static vector<ofVec2f> DPOINTS;
	static segment DSEG;
	
	vector <segment> tracks;
	vector <node> nodes;
	vector <blip> blips;
	
	segment previewTracks[2];
	blip previewBlip;
	
	node * s_nodes[2];
	segment * s_tracks[2];
	ofVec2f s_pos[2];
	bool nodeSet[4];
	
	ofVec2f insertPoint;
	ofVec2f insertDir;
	float insertSize;
	bool isInsert;
	
	bool isPreview;
	
};

#endif