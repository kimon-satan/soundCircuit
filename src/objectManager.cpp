/*
 *  objectManager.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 04/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "objectManager.h"

bool nodeSuperfluous(node n){return n.getSuperfluous();} //used in endTrack
bool trackIsInvalid(segment s){return !s.getIsValid();} //use in insertSpace

segment objectManager::DSEG = segment();
vector<ofVec2f> objectManager::DPOINTS = vector<ofVec2f>();

objectManager::objectManager():objectUtils(){
	
	isPreview = false;
	isInsert = false;
	
}

//accessible functions

void objectManager::beginTrack(ofVec2f w_pos){
	
	isPreview = true;
	
	deselectNodes();
	deselectTracks();
	
	s_nodes[0] = selectNode(w_pos, kTestSize/2);
	
	if(s_nodes[0]){
		w_pos = s_nodes[0]->getPos();
	}else{
		
		s_tracks[0] = selectTrackPoint(w_pos);
		if(s_tracks[0])w_pos = s_tracks[0]->getSelectPos();
	}
	
	previewTracks[0].setStartPos(w_pos);
	s_pos[0] = w_pos;
}

void objectManager::calcTrack(ofVec2f w_pos, ofVec2f w_dir, int mode){
	
	if(w_dir.length() > WORLD_UNIT * 5){ //t_dir is a pixel value
		
		if(mode == 0){
			calcTrack_0(w_pos, w_dir);
		}else{
			calcTrack_1(w_pos, w_dir);
		}
		
	}
	
}

void objectManager::endTrack(){
	
	if(!previewTracks[0].getIsValid() && !previewTracks[1].getIsValid())return;
	
	int numTracks = (previewTracks[1].getIsValid()) ? 2 : 1; 
	
	vector<node *> currentNodes;
	
	for(int j = 0; j < numTracks; j ++){
		
		tracks.push_back(previewTracks[j]);
		tracks.back().aquireIndex();
		addNodesToTrack(tracks.back());
		previewTracks[j].setIsValid(false);
		
	}
	
	deselectNodes();
	deselectTracks();
	isPreview = false;
	
	//clean up
	
	vector<node>::iterator it = remove_if(nodes.begin(), nodes.end(), nodeSuperfluous);
	nodes.erase(it, nodes.end());
	
	
}



ofVec2f objectManager::flipBoundaryPoint(ofVec2f t){

	ofVec2f flipped(t);
	
	if(flipped.x >= world_dims.x/2 - kTestSize/2){
		flipped.x -= world_dims.x;
	}else if(flipped.x <= -world_dims.x/2 + kTestSize/2){
		flipped.x += world_dims.x;
	}
	
	if(flipped.y >= world_dims.y/2 - kTestSize/2){
		flipped.y -= world_dims.y;
	}else if(flipped.y <= -world_dims.y/2 + kTestSize/2){
		flipped.y += world_dims.y;
	}
	
	return flipped;
	
}


void objectManager::beginBlip(ofVec2f w_pos, blipPreset bp){
	
	deselectTracks();
	deselectNodes();
	
	s_tracks[0] = selectTrackPoint(w_pos);
	
	if(!s_tracks[0]){
		return;
	}else if(selectBlip(w_pos, 3)){
		return;
	}else if(selectNode(w_pos, kTestSize/2)){
		return;
	}else{
		s_pos[0] = s_tracks[0]->getSelectPos();
		previewBlip.setPreset(bp);
		previewBlip.createDrawer(world_dims);
		calcBlip(s_pos[0], ofVec2f(0,0), 0);
		previewBlip.getPresetRef().setIsRandSet(true);
	}
	
	
}

void objectManager::calcBlip(ofVec2f w_pos, ofVec2f t_dir, float s_angle){

	if(!s_tracks[0]){
		previewBlip.setIsValid(false);
		return;
	}
	
	if(!previewBlip.getDrawer()){
		previewBlip.setIsValid(false);
		return; //a new blip hasn't been started
	}
	
	
	//preset parameter sorting
	float m_val = (float)WORLD_UNIT * M_VAL;
	float userA = min(m_val,t_dir.length());
	float userB = s_angle;
	userB = abs(userB);
	
	if(userA == 0){
		userA = previewBlip.getPreset().getUserVals().at(0);
		userB = previewBlip.getPreset().getUserVals().at(1);
	}
	
	blipPreset & p = previewBlip.getPresetRef();
	
	setParam(p.getLength(),userA, userB, m_val);
	setParam(p.getAttackSecs(),userA, userB, m_val);
	setParam(p.getDecaySecs(),userA, userB, m_val);
	setParam(p.getPostDecaySecs(),userA, userB, m_val);
	for(int i = 0; i < p.getSoundParams()->size(); i++)setParam(p.getSoundParam(i), userA, userB, m_val);
	for(int i = 0; i < p.getVisualParams()->size(); i++)setParam(p.getVisualParam(i), userA, userB, m_val);
	
	
	if(s_tracks[0] && !selectBlip(s_pos[0],3)){
		
		ofVec2f tt_dir(s_tracks[0]->getDirection() * p.getLength()->abs_value);
		s_pos[1] = s_pos[0] - tt_dir/2;
		makeSegment(s_pos[1], tt_dir, previewBlip); //look here !
		
		if(p.getLength()->setType == PSET_USERA || p.getLength()->setType == PSET_USERB ){
			constrainFromMidPoint(s_pos[0], previewBlip, false, true, true);
		}else{
			repositionFromMidPoint(s_pos[0], previewBlip, false, true, true);
		}
		
		//checking
		bool recalc = false;
		if(!recalc){
			if(selectBlip(previewBlip.getStartPos())||selectBlip(previewBlip.getEndPos())){
				recalc =true;
			}
		}
		if(!recalc){
			vector<ofVec2f> points;
			findNodeIntersects(previewBlip, points);
	
			for(int i = 0; i < points.size(); i ++){
				if(points[i] != previewBlip.getStartPos() || points[i] != previewBlip.getEndPos()){
					recalc = true; 
					break;
				}
			}
			
		}
		
		if(previewBlip.getLength() < p.getLength()->min_val)recalc = true;
		
		previewBlip.updateDrawer();
		previewBlip.updateDrawerPosition(world_dims); //needs to be called as blip position can change
		previewBlip.updateDrawer();
		previewBlip.setIsValid(!recalc);

	}
	
}


void objectManager::setParam(paramAttributes * p, float userA, float userB, float m_val){

	if(p->setType == PSET_USERA){
		
		p->abs_value = ofMap(userA, 0, m_val, p->min_val, p->max_val);
		
	}else if(p->setType == PSET_USERB){
		
		p->abs_value = ofMap(userB, 0, 180, p->min_val, p->max_val);
		
	}else if(p->setType == PSET_MAP){
		
		float mapValue = 0;
		//map to world position
		if(s_tracks[0]->getDirection().x > 0){
			mapValue = ofMap(s_pos[0].x, -world_dims.x/2, world_dims.x/2, 
							 p->min_val, p->max_val);
		}else{
			mapValue = ofMap(s_pos[0].y, -world_dims.y/2, world_dims.y/2, 
							 p->min_val, p->max_val);
		}
		
		p->abs_value = mapValue;
		
	}else if(p->setType == PSET_RANDOM){
		
		if(!previewBlip.getPreset().getIsRandSet()){
			p->abs_value = ofRandom(p->min_val, p->max_val);
		}
		//potentially throw in some other distributions PSET_NORMAL, PSET_SKEW // an extra attribute required 
	
	}else if(p->setType == PSET_SLAVE){
		
		if(p->slaveTo != ""){
		
			paramAttributes * master = NULL;
			if(p->slaveTo == "length")master = previewBlip.getPreset().getLength();
			if(p->slaveTo == "attackSecs")master = previewBlip.getPreset().getAttackSecs();
			if(p->slaveTo == "decaySecs")master = previewBlip.getPreset().getDecaySecs();
			if(!master){master = previewBlip.getPreset().getSoundParam(p->slaveTo);}
			if(!master)master = previewBlip.getPreset().getVisualParam(p->slaveTo);
			if(master){
				
				if(master->max_val < master->min_val){
					p->abs_value = ofMap(master->abs_value, master->max_val, master->min_val, p->min_val, p->max_val);
				}else{
					p->abs_value = ofMap(master->abs_value, master->min_val, master->max_val, p->min_val, p->max_val);
				}
				
			}

		}
		
	
	}
	
	
}

void objectManager::endBlip(){
	
	if(previewBlip.getIsValid()){
		blips.push_back(previewBlip);
		blips.back().createDrawer(world_dims, previewBlip.getDrawer());
		blips.back().aquireIndex();
	}
	previewBlip.setIsValid(false);
	previewBlip.destroyDrawer();
	
}

void objectManager::beginNode(ofVec2f w_pos ,int mode){

	deselectNodes();
	deselectTracks();
	s_nodes[0] = selectNode(w_pos, kTestSize/2);
	if(s_nodes[0])s_nodes[0]->setIsAdjusting(true);
	for(int i = 0; i < 4; i++)nodeSet[i]=false;
	
	if(s_nodes[0]){ //open the node entirely
		if(mode != 0){
			for(int i = 0; i < 4; i++)s_nodes[0]->openSocket(i);
		}	
	}
	
}


void objectManager::calcNode(ofVec2f w_pos){
	
	if(!s_nodes[0])return;
	
	float dist = w_pos.distance(s_nodes[0]->getPos());
	
	if(dist > kTestSize){
		//try flipping it
		w_pos = flipBoundaryPoint(w_pos);
		dist = w_pos.distance(s_nodes[0]->getPos());
		
	}
	
	if(dist < kTestSize * (float)WORLD_UNIT * 0.75f && 
	   dist > kTestSize * (float)WORLD_UNIT * 0.25f
	   ){
		
		ofVec2f w_dir(w_pos - s_nodes[0]->getPos());
		quantizeDirection(w_dir);
		int i = node::getSocketIndex(w_dir);
		
		if(s_nodes[0] && !nodeSet[i]){
			
			if(s_nodes[0]->getNowSocket(i)){
				s_nodes[0]->closeSocket(i);
			}else {
				s_nodes[0]->openSocket(i);
			}
			
			nodeSet[i] = true;
			for(int j = i + 1; j < i + 4; j++)nodeSet[j%4]=false; //set all the others to false
		}
	}else{
		
		for(int i = 0; i < 4; i++)nodeSet[i]=false;
	
	}
	
}

void objectManager::endNode(){
	
	deselectNodes();
	
}


//protected constructors

void objectManager::calcTrack_0(ofVec2f w_pos, ofVec2f t_dir){

	
	if(s_nodes[1]){
		if(s_nodes[0] != s_nodes[1])s_nodes[1]->setSelected(false);
		s_nodes[1] = NULL;
	}
	
	
	if(s_tracks[1]){
		s_tracks[1]->setSelected(false);
		s_tracks[1] = NULL;
	}
	
	ofVec2f t_pos_0(s_pos[0]); //copied to local so as not lose original sp
	
	s_nodes[1] = selectNode(w_pos,kTestSize/2);

	if(!s_nodes[1])s_tracks[1] = selectTrackPoint(w_pos);
	
	bool recalc = false;
	ofVec2f size(t_dir);
	
	if(!s_nodes[1] && !s_tracks[1]){
		
	singleTrack:
		
	
		makeSegment(t_pos_0, size, previewTracks[0]);
		constrainEndPoint(t_pos_0, previewTracks[0]);
		
		recalc = findParalellIntersects(previewTracks[0]);
		if(!recalc){
			if(selectBlip(previewTracks[0].getStartPos())||selectBlip(previewTracks[0].getEndPos()))recalc =true;
				}
		
		if(!recalc)recalc = findNodeIntersects(previewTracks[0]);
		
		if(previewTracks[0].getLength() < WORLD_UNIT * MIN_TRACK){
			recalc = true;
		}
		
		if(recalc){
			for(int j = 0; j < 2; j ++)previewTracks[j].setIsValid(false);
		}else{
			previewTracks[0].setIsValid(true);
			previewTracks[1].setIsValid(false);
		}
		
		
		
		return;
		
	}
	
	
	//prototype 2 tracks
	
	ofVec2f dirs[2], sps[2];
	
	s_pos[1] = (s_nodes[1]) ? s_nodes[1]->getPos(): s_tracks[1]->getSelectPos();
	
	size.set(s_pos[1] - t_pos_0);
	
	sps[0].set(t_pos_0);
	
	ofVec2f q(size/t_dir); //negative q value indicates a wrap
	q.normalize();
	
	//wrapping adjustment for lengths
	if(q.x < 0 && abs(t_dir.x) > WORLD_UNIT * 10){
		if(size.x > 0){size.x -= world_dims.x;}else{size.x += world_dims.x;}
	}
	
	if(q.y < 0 && abs(t_dir.y) > WORLD_UNIT * 10){
		if(size.y > 0){size.y -= world_dims.y;}else{size.y += world_dims.y;}
	}
	
	dirs[0].set(0, size.y); 
	dirs[1].set(size.x,0);
	
	if(abs(size.x) < kTestSize/2 * WORLD_UNIT || abs(size.y) < kTestSize/2 * WORLD_UNIT){ //if nearly aligned try a single track first
		
		if(!s_nodes[1]){
			
			goto singleTrack;
			
		}else if(!s_nodes[0]){
			
			if(abs(size.x) > 0){
				t_pos_0.y = s_pos[1].y; 
			}else{
				t_pos_0.x = s_pos[1].x;
			}
			
			goto singleTrack;
			
		}else if(size.x == 0 || size.y ==0){
			
			goto singleTrack;
			
		}else{
			
			for(int j = 0; j < 2; j ++)previewTracks[j].setIsValid(false);		
			return;
		}
		
	}
	
	
	for(int i = 0; i < 2; i++){ //need to check about occasional crazy wraps
		
		recalc = false;
		
		sps[1].set(ofVec2f(s_pos[i].x, s_pos[(i+1)%2].y));
		
		for(int j = 0; j < 2; j ++){
			makeSegment(sps[j], dirs[(j + i)%2], previewTracks[j]); 
			segment s = previewTracks[j];
			constrainEndPoint(sps[i],s);
			if(s.getLength() < previewTracks[j].getLength()){
				if(i == 0){
					recalc = true;
				}else if(i == 1){
					goto singleTrack;
				}
			}
			if(!recalc)recalc = findParalellIntersects(previewTracks[j]);
			if(!recalc)recalc = findNodeIntersects(previewTracks[j]);
			if(!recalc){
				if(selectBlip(previewTracks[j].getStartPos())||selectBlip(previewTracks[j].getEndPos()))recalc =true;
			}
			
		}
		
		if(!recalc){
			for(int j = 0; j < 2; j ++)previewTracks[j].setIsValid(true);
			return;
		}
		
	}
	
	if(recalc)goto singleTrack;
	
}

void objectManager::calcTrack_1(ofVec2f w_pos, ofVec2f t_dir){
	
	deselectNodes();
	deselectTracks();
	
	if(selectTrackPoint(s_pos[0])){
		previewTracks[0].setIsValid(false);
		return;
	}
	
	quantizeDirection(t_dir);
	vector<ofVec2f> intersects;
	findCrossIntersects(s_pos[0], t_dir, intersects);
	vector<ofVec2f> bounds = getPlanarNeighbours(s_pos[0], intersects);
	
	t_dir/= t_dir;
	t_dir *= world_dims;
	ofVec2f n_dir(t_dir);
	
	if(bounds[0] != bounds[1]){
		n_dir = bounds[1]-bounds[0];
		if(n_dir.x + n_dir.y < 0){
			n_dir = t_dir + n_dir;
		}
	}
	
	makeSegment(bounds[0], n_dir, previewTracks[0]);
	
	bool recalc = false;

	if(!recalc)recalc = findParalellIntersects(previewTracks[0]);
	if(!recalc)recalc =findNodeIntersects(previewTracks[0]);
	if(!recalc){
		if(selectBlip(previewTracks[0].getStartPos())||selectBlip(previewTracks[0].getEndPos()))recalc =true;
	}
	if(!recalc)previewTracks[0].setIsValid(true);
	
}


void objectManager::addNodesToTrack(segment & s){
	
	for(int i = 0; i < 2; i ++){
		
		ofVec2f pos = (i == 0)? s.getStartPos():s.getEndPos();
		int mul = (i == 0) ? 1 : -1;
		
		//first check if node is already created
		node * n = selectNode(pos, kTestSize/4);
		
		if(!n){
			node t(pos);
			t.addSocket(s.getDirection() * mul);
			t.openSocket(s.getDirection() * mul);
			nodes.push_back(t);
			n = &nodes.back();
			
			//in case new node lies on an existing track
			segment * ts = selectTrackPoint(n->getPos());
			if(ts){
				if(ts->getIndex() != s.getIndex()){
					n->addSocket(ts->getDirection()); //potential risk of reader skipping off track ? 
					n->openSocket(ts->getDirection());
					n->addSocket(-ts->getDirection());
					n->openSocket(-ts->getDirection());
					n->closeSocket(s.getDirection() * mul); //close the socket as default
				}
			}
			
			//in case new node is on a bridge point
			ofVec2f t_pos(pos - s.getDirection() *  WORLD_UNIT * 5 * mul); // point slightly outside the track
			ts = selectTrackPoint(t_pos);
			if(ts){
				n->addSocket(-ts->getDirection() * mul);
				n->openSocket(-ts->getDirection() * mul);
			}
			
			
		}else{
			n->addSocket(s.getDirection() * mul);
			if(n->getNumSockets() < 3)n->openSocket(s.getDirection() * mul); //otherwise leave as is
		}
		
		

		
	}
	
}

void objectManager::constrainEndPoint(ofVec2f origin, segment & s, bool isTracks, bool isNodes, bool isBlips){
	
	vector<ofVec2f> points;
	
	findCrossIntersects(origin, s.getDirection(), points);
	
	if(points.size() > 0)limitEndPointFromStart(origin, points, s);
		
}

void objectManager::constrainFromMidPoint(ofVec2f origin, segment & s, bool isTracks, bool isNodes, bool isBlips){
	
	vector<ofVec2f> points;
	
	if(isNodes)findNodeIntersects(s, points, true);
	if(isBlips)findParalellIntersects(s, points, false, true);
	
	if(points.size() > 0){limitStartPointFromMid(origin, points, s);}
	
	points.clear();
	
	if(isNodes)findNodeIntersects(s, points, true);
	if(isBlips)findParalellIntersects(s, points, false, true);
	
	if(points.size() > 0){limitEndPointFromMid(origin, points, s);}
		
}

void objectManager::repositionFromMidPoint(ofVec2f origin, segment & s, bool isTracks, bool isNodes, bool isBlips){

	vector<ofVec2f> points;
	
	if(isNodes)findNodeIntersects(s, points, true);
	if(isBlips)findParalellIntersects(s, points, false, true);
	
	ofVec2f old_sp(s.getStartPos());
	
	if(points.size() > 0){limitStartPointFromMid(origin, points, s);}
	
	if(old_sp == s.getStartPos()){
	
		points.clear();
		
		if(isNodes)findNodeIntersects(s, points, true);
		if(isBlips)findParalellIntersects(s, points, false, true);
		
		if(points.size() > 0){
			limitEndPointFromMid(origin, points, s, true);
		}
	}

	
}


void objectManager::beginInsertSpace(ofVec2f t_point, ofVec2f t_dir, float t_size){

	beginInsertion(t_point, t_dir);
	
	vector<segment> newTracks;
	
	for(int i = 0; i < tracks.size(); i++){
		
		if(tracks[i].getDirection() == insertDir){
			
			ofVec2f testPoint(tracks[i].getStartPos() * ofVec2f(insertDir.y,insertDir.x) + insertDir * insertPoint);
			
			if(tracks[i].getInside(testPoint)){
				
				blip * tb = selectBlip(testPoint, kTestSize);
				if(tb)splitBlip(*tb, testPoint);
				segment n_seg = splitSegment(tracks[i], testPoint, 1);
				if(n_seg.getLength() > MIN_TRACK * WORLD_UNIT){
					n_seg.aquireIndex();
					n_seg.setIsValid(true);
					newTracks.push_back(n_seg);
				}else{
					node * n = selectNode(n_seg.getEndPos(), (float)kTestSize/4);
					if(n)n->removeSocket(-n_seg.getDirection());
				}
				
				if(tracks[i].getLength() > MIN_TRACK * WORLD_UNIT){
					newTracks.push_back(tracks[i]);
				}else{
					node * n = selectNode(tracks[i].getStartPos(), (float)kTestSize/4);
					if(n)n->removeSocket(tracks[i].getDirection());
				}
				
				tracks[i].setIsValid(false);
				
			}
		}
	}
	
	
	//remove all the unsused tracks
	
	vector<segment>::iterator it = remove_if(tracks.begin(), tracks.end(), trackIsInvalid);
	tracks.erase(it, tracks.end());
	for(it = newTracks.begin(); it != newTracks.end(); it++)tracks.push_back(*it);
	
	resizeInsertion(t_size);
	insertSize = 0;

	for(int i = 0; i < tracks.size(); i ++)addNodesToTrack(tracks[i]);
	
	vector<node>::iterator n_it = remove_if(nodes.begin(), nodes.end(), nodeSuperfluous);
	nodes.erase(n_it, nodes.end());

}


	

void objectManager::splitBlip(blip & b, ofVec2f t_point){
		
	segment t_seg = splitSegment(b, t_point, kTestSize/2);
	blipPreset bp = b.getPreset();
	float min_len = bp.getLength()->min_val;
	
	if(b.getLength() > min_len){
		updateBlipDraw(b);
	}else{
		destroyBlip(b);
	}
	
	if(t_seg.getLength() > min_len){
		blip n_b;
		n_b.setPreset(bp);
		
		//add update from segment into blip
		n_b.setStartPos(t_seg.getStartPos());
		n_b.setLength(t_seg.getLength());
		n_b.setDirection(t_seg.getDirection());
		//--------------------------------
		
		updateTestAreas(n_b);
		blips.push_back(n_b);
		blips.back().aquireIndex();
		blips.back().createDrawer(world_dims);
	}
		
	
}

segment objectManager::splitSegment(segment & s, ofVec2f s_point, float splitRadius){

	//to do add a method to handle when node intersect
	
	//the old segment
	ofVec2f o_ep = s_point - s.getDirection();
	ofVec2f o_vec = o_ep - s.getStartPos();
	if(o_vec.x + o_vec.y < 0){o_vec += world_dims * s.getDirection();} //wrap correction
	float t_length = s.getLength();
	s.setLength(o_vec.length()- splitRadius);
	updateTestAreas(s);
	
	//the new segment
	segment n_seg = s;
	float n_length = t_length - (o_vec.length()); //the remainder is the new length
	n_seg.setLength(n_length - splitRadius);
	ofVec2f n_sp = s_point + s.getDirection() * splitRadius;
	n_seg.setStartPos(n_sp);
	updateTestAreas(n_seg);
	
	return n_seg;
	

}


void objectManager::beginInsertion(ofVec2f t_point, ofVec2f t_dir){

	insertPoint.set(t_point);
	insertDir.set(t_dir);
	isInsert = true;
	insertSize = 0;

}

void objectManager::resizeInsertion(float size){
	
	float diff = size - insertSize;
	world_dims += insertDir * diff * 2;
	insertSize = size;
	
	float testInsert = insertPoint.x * insertDir.x + insertPoint.y * insertDir.y;
	
	for(int i = 0; i < nodes.size();i++){
		
		ofVec2f t_pos(nodes[i].getPos());
		float testPos = t_pos.x * insertDir.x + t_pos.y * insertDir.y;
		t_pos += (testPos > testInsert)? diff * insertDir: -diff * insertDir;
		nodes[i].setPos(t_pos);
		
	}
	
	for(int i = 0; i < tracks.size(); i++){
				
		ofVec2f t_pos(tracks[i].getStartPos());
		float testPos = t_pos.x * insertDir.x + t_pos.y * insertDir.y;
		t_pos += (testPos > testInsert)? diff * insertDir: -diff * insertDir;
		tracks[i].setStartPos(t_pos);
			
		if(tracks[i].getDirection() == insertDir){
			
			ofVec2f testPoint(tracks[i].getStartPos() * ofVec2f(insertDir.y,insertDir.x) + insertDir * insertPoint);
			
			//test with a spread of points so as no chance of creating gaps on bridged tracks
			//leads to overlaps but seems ok
			
			if(tracks[i].getInside(testPoint)|| 
			   tracks[i].getInside(testPoint + insertDir * 1) ||
			   tracks[i].getInside(testPoint - insertDir * 1))
			{
				float length = tracks[i].getLength() + diff * 2;
				tracks[i].setLength(length);
				
			}
		}
		
		updateTestAreas(tracks[i]);
		
	}
	
	
	
	for(int i = 0; i < blips.size(); i++){
		
		ofVec2f t_pos = blips[i].getStartPos();
		float testPos = t_pos.x * insertDir.x + t_pos.y * insertDir.y;
		t_pos += (testPos > testInsert)? diff * insertDir: -diff * insertDir;
		blips[i].setStartPos(t_pos);
		updateBlipDraw(blips[i]);
		
	}
	

}

void objectManager::updateBlipDraw(blip & tb){

	//to reset the drawer
	updateTestAreas(tb);
	tb.updateDrawer();
	tb.updateDrawerPosition(world_dims);
	tb.updateDrawer();

}

void objectManager::endInsertion(){

	isInsert = false;
	
}

//searches


bool objectManager::findCrossIntersects(ofVec2f origin, ofVec2f t_dir, vector<ofVec2f> & t_points, segment & s, bool isTracks, bool isBlips){

	t_dir /= t_dir;
	bool isIntersect = false;

	if(isTracks){
		for(int i  = 0; i < tracks.size(); i ++){
			
			if(t_dir != tracks[i].getDirection()){
				
				ofVec2f t_pos(origin * ofVec2f(t_dir.y,t_dir.x) + tracks[i].getStartPos() * t_dir);
				
				
				if(tracks[i].getInside(t_pos) 
				   || tracks[i].getInside(t_pos + tracks[i].getDirection() * (float)kTestSize/4 * WORLD_UNIT) 
				   || tracks[i].getInside(t_pos - tracks[i].getDirection() * (float)kTestSize/4 * WORLD_UNIT)
				   ){
					
					if(&s == &DSEG || s.getInside(t_pos)){
						if(&t_points != &DPOINTS){
							t_points.push_back(t_pos);
							isIntersect = true;
							
						}else{
							return true;
						}
					}
					
				}
			}
		}
	}
	
	if(isBlips){
		for(int i  = 0; i < blips.size(); i ++){
			
			if(t_dir != blips[i].getDirection()){
				
				ofVec2f t_pos(origin * ofVec2f(t_dir.y,t_dir.x) + blips[i].getStartPos() * t_dir);
				
				if(blips[i].getInside(t_pos)){
					
					if(&s == &DSEG || s.getInside(t_pos)){
						if(&t_points != &DPOINTS){
							t_points.push_back(t_pos);
							isIntersect = true;
						}else{
							return true;
						}
					}
					
				}
			}
		}
	
	}
	
	
	return isIntersect;
	
}

bool objectManager::findParalellIntersects(segment & s, vector<ofVec2f> & t_points, bool isTracks, bool isBlips){
	
	bool isIntersect = false;
	
	if(isTracks){
		for(int i = 0; i < tracks.size(); i++){ 
			
			if(s.getDirection() == tracks[i].getDirection()){
				
				if(tracks[i].getInside(s.getStartPos())||
				   tracks[i].getInside(s.getEndPos())||
				   s.getInside(tracks[i].getStartPos())||
				   s.getInside(tracks[i].getEndPos())
				   ){
					
					if(&t_points != &DPOINTS){
						isIntersect = true;
						t_points.push_back(tracks[i].getStartPos());
						t_points.push_back(tracks[i].getEndPos());

						
					}else{
						return true;
					}
				}
				
			}
		}
	}
	
	if(isBlips){
		for(int i = 0; i < blips.size(); i++){ 
			
			if(s.getDirection() == blips[i].getDirection()){
				
				if(blips[i].getInside(s.getStartPos())||
				   blips[i].getInside(s.getEndPos())||
				   s.getInside(blips[i].getStartPos())||
				   s.getInside(blips[i].getEndPos())
				   ){
				
					if(&t_points != &DPOINTS){
						isIntersect = true;
						t_points.push_back(blips[i].getStartPos());
						t_points.push_back(blips[i].getEndPos());
					}else{
						return true;
					}
				}
				
			}
		}
	}
	
	
	return isIntersect;
	
}

bool objectManager::findNodeIntersects(segment & s, vector<ofVec2f> & t_points, bool includeTestArea){
	
	bool isIntersect = false;
	float rad =  kTestSize * (float)WORLD_UNIT/2;
	
	for(int i = 0; i < nodes.size(); i++){
		
		if(!nodes[i].getIsSelected()){
			
			ofVec2f n_pos(nodes[i].getPos());
			ofVec2f f_pos(flipBoundaryPoint(nodes[i].getPos()));
			ofVec2f t_pos(s.getInside(n_pos)? n_pos : f_pos);
						  
			if(s.getInside(t_pos)){
				
				if(&t_points != &DPOINTS){
					
					if(includeTestArea){
						
						ofVec2f tmp(t_pos +  s.getDirection() * rad);
						if(s.getInside(tmp))t_points.push_back(tmp);
						tmp.set(t_pos -  s.getDirection() * rad);
						if(s.getInside(tmp))t_points.push_back(tmp);
						tmp.set(f_pos +  s.getDirection() * rad);
						if(s.getInside(tmp))t_points.push_back(tmp);
						tmp.set(f_pos -  s.getDirection() * rad);
						if(s.getInside(tmp))t_points.push_back(tmp);
						
					}else{
					   t_points.push_back(t_pos);
					}
					isIntersect = true;
				}else{
					return true;
					
				}
				
			}else if(checkNode(nodes[i], s.getStartPos(), rad) || checkNode(nodes[i], s.getEndPos(), rad)){
				
				if(&t_points != &DPOINTS){
					isIntersect = true;
					
					if(includeTestArea){
						ofVec2f tmp(n_pos +  s.getDirection() * rad);
						if(s.getInside(tmp))t_points.push_back(tmp);
						tmp.set(n_pos -  s.getDirection() * rad);
						if(s.getInside(tmp))t_points.push_back(tmp);
						tmp.set(f_pos +  s.getDirection() * rad);
						if(s.getInside(tmp))t_points.push_back(tmp);
						tmp.set(f_pos -  s.getDirection() * rad);
						if(s.getInside(tmp))t_points.push_back(tmp);
					}
				}else{
					return true;
				}
				
			}
		}
	}
	
	return isIntersect;

}




//selection tools

void objectManager::deselectNodes(){
	
	for(int i = 0; i < nodes.size(); i ++){
		nodes[i].setSelected(false);
		nodes[i].setIsAdjusting(false);
	}
	s_nodes[0] = NULL; s_nodes[1] = NULL;
	
}

void objectManager::deselectTracks(){
	
	for(int i = 0; i < tracks.size(); i ++)tracks[i].setSelected(false);
	s_tracks[0] = NULL; s_tracks[1] = NULL;
}

node * objectManager::selectNode(ofVec2f t_pos, int rad){
	
	node * n = NULL;
	
	for(int i = 0; i < nodes.size(); i++){
		
		if(checkNode(nodes[i], t_pos, rad)){
			n = &nodes[i];
			nodes[i].setSelected(true);
			break;
		}
	}
	
	return n;
}

bool objectManager::checkNode(node & n, ofVec2f t_pos, int rad){

	ofVec2f f_pos(flipBoundaryPoint(t_pos));
	if( t_pos.distance(n.getPos()) < rad * (float)WORLD_UNIT ||
	   f_pos.distance(n.getPos()) < rad * (float)WORLD_UNIT 
	   ){
		return true;
	}
	
	return false;
}

segment * objectManager::selectTrackPoint(ofVec2f t_pos){
	
	segment * t = NULL;
	ofVec2f f_pos(flipBoundaryPoint(t_pos));
	
	for(int i = 0; i < tracks.size(); i++){
		if(tracks[i].getInside(t_pos)){
			t = &tracks[i];
			t->setSelected(true, t_pos);
			break;
		}else if(tracks[i].getInside(f_pos)){
			t = &tracks[i];
			t->setSelected(true, f_pos);
			break;
		}
	}
	
	return t;
}

blip * objectManager::selectBlip(ofVec2f t_pos, int bZone){
	
	blip * t = NULL;
	
	for(int i = 0; i < blips.size(); i++){
		if(blips[i].getInside(t_pos, bZone)){
			t = &blips[i];
			break;
		}
	}
	
	return t;
}

void objectManager::destroyBlip(blip & tb){

	vector<blip>::iterator it = remove_if(blips.begin(), blips.end(), bind2nd(blipIndex(), tb.getIndex()));
	blips.erase(it, blips.end());
	
}

void objectManager::destroyTrack(segment & ts){

	vector<segment>::iterator it = remove_if(tracks.begin(), tracks.end(), bind2nd(segmentIndex(), ts.getIndex()));
	//potentially erase blips on the track depending on use
	tracks.erase(it, tracks.end());

}


vector<string> objectManager::getPreviewParams(){
	
	vector<string> paramString;
	blipPreset p = previewBlip.getPreset();
	
	if(p.getAttackSecs()->setType == PSET_USERA || p.getAttackSecs()->setType == PSET_USERB){
		string t;
		t = p.getAttackSecs()->name;
		t +=  " : " + ofToString(p.getAttackSecs()->abs_value, 2);
		paramString.push_back(t);
	}
	
	if(p.getDecaySecs()->setType == PSET_USERA || p.getDecaySecs()->setType == PSET_USERB){
		string t;
		t =  p.getDecaySecs()->name;
		t +=  " : " + ofToString(p.getDecaySecs()->abs_value, 2);
		paramString.push_back(t);
	}
	
	
	for(int i = 0; i < p.getSoundParams()->size(); i++){
		
		if(p.getSoundParam(i)->setType == PSET_USERA || p.getSoundParam(i)->setType == PSET_USERB){
			string t;
			t =  p.getSoundParams()->at(i).name;
			t +=  " : " + ofToString(p.getSoundParams()->at(i).abs_value, 2);
			paramString.push_back(t);
		}
	}
	
	return paramString;
}


//getters and setters


