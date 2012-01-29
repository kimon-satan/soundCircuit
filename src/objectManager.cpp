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
segment objectManager::DSEG = segment();
vector<ofVec2f> objectManager::DPOINTS = vector<ofVec2f>();

objectManager::objectManager(){
	
	isPreview = false;
}

//accessible functions

void objectManager::beginTrack(ofVec2f w_pos){
	
	isPreview = true;
	
	deselectNodes();
	deselectTracks();
	s_nodes[0] = selectNode(w_pos);
	
	if(s_nodes[0]){
		w_pos = s_nodes[0]->getPos();
	}else{
		
		s_tracks[0] = selectTrackPoint(w_pos);
		if(s_tracks[0])w_pos = s_tracks[0]->getSelectPos();
	}
	
	previewTracks[0].setStartPos(w_pos);
	s_pos[0] = w_pos;
}

void objectManager::calcTrack(ofVec2f w_pos, ofVec2f t_dir, int mode){
	
	if(t_dir.length() > 10){
		
		if(mode == 0){
			calcTrack_0(w_pos, t_dir);
		}else{
			calcTrack_1(w_pos, t_dir);
		}
		
	}
	
}

void objectManager::endTrack(){
	
	if(!previewTracks[0].getIsValid() && !previewTracks[1].getIsValid())return;
	
	int numTracks = (previewTracks[1].getIsValid()) ? 2 : 1; 
	
	vector<node *> currentNodes;
	
	for(int j = 0; j < numTracks; j ++){
		
		tracks->push_back(previewTracks[j]);
		tracks->back().aquireIndex();
		
		for(int i = 0; i < 2; i ++){
			
			
			ofVec2f pos = (i == 0)? previewTracks[j].getStartPos():previewTracks[j].getEndPos();
			int mul = (i == 0) ? 1 : -1;
			
			//first check if node is already created
			node * n = selectNode(pos);
			
			if(!n){
				node t(pos);
				t.openSocket(previewTracks[j].getDirection() * mul);
				nodes->push_back(t);
				n = &nodes->back();
				
			}else{
				n->openSocket(previewTracks[j].getDirection() * mul);
			}
			
			//in case new node lies on an existing track
			segment * s = selectTrackPoint(n->getPos());
			if(s){
				n->openSocket(s->getDirection()); //potential risk of reader skipping off track ? 
				n->openSocket(-s->getDirection());
			}
			
		}
		
		previewTracks[j].setIsValid(false);
		
	}
	
	deselectNodes();
	deselectTracks();
	isPreview = false;
	
	//clean up
	
	vector<node>::iterator it = remove_if(nodes->begin(), nodes->end(), nodeSuperfluous);
	nodes->erase(it, nodes->end());
	
	
}

void objectManager::beginBlip(ofVec2f w_pos, blipPreset bp){
	
	deselectTracks();
	deselectNodes();
	
	s_tracks[0] = selectTrackPoint(w_pos);
	
	if(!s_tracks[0]){
		return;
	}else if(selectBlip(w_pos)){
		return;
	}else{
		s_pos[0] = s_tracks[0]->getSelectPos();
		previewBlip.setPreset(bp);
		calcBlip(s_pos[0], ofVec2f(0,0));
		previewBlip.createDrawer(world_dims);
		previewBlip.getPresetRef().setIsRandSet(true);
	}
	
	
}

void objectManager::calcBlip(ofVec2f w_pos, ofVec2f t_dir){

	if(!s_tracks[0]){
		previewBlip.setIsValid(false);
		return;
	}
	
	//preset parameter sorting

	float userA = t_dir.length();
	float userB = t_dir.angle(ofVec2f(0,1));
	userB = abs(userB);
	
	if(userA == 0){
		userA = previewBlip.getPreset().getUserVals().at(0);
		userB = previewBlip.getPreset().getUserVals().at(1);
	}
	
	blipPreset & p = previewBlip.getPresetRef();
	
	float m_val = ofVec2f(ofGetScreenWidth()/2, ofGetScreenHeight()/2).length();


	setParam(p.getLength(),userA, userB, m_val);
	setParam(p.getAttackSecs(),userA, userB, m_val);
	setParam(p.getDecaySecs(),userA, userB, m_val);
	setParam(p.getPostDecaySecs(),userA, userB, m_val);
	for(int i = 0; i < p.getSoundParams()->size(); i++)setParam(p.getSoundParam(i), userA, userB, m_val);
	for(int i = 0; i < p.getVisualParams()->size(); i++)setParam(p.getVisualParam(i), userA, userB, m_val);
	
	if(s_tracks[0] && !selectBlip(s_pos[0])){
		
		ofVec2f t_dir(s_tracks[0]->getDirection() * p.getLength()->abs_value);
		s_pos[1] = s_pos[0] - t_dir/2;
		makeSegment(s_pos[1], t_dir, previewBlip);
		
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
					recalc = true; break;
				}
			}
			
		}
		if(previewBlip.getLength() < p.getLength()->min_val){
			recalc = true;
		}
		if(!recalc){
			previewBlip.setIsValid(true);
			previewBlip.updateDrawerPosition(world_dims); //needs to be called as blip position can change
			previewBlip.updateDrawer();
		}else{
			previewBlip.setIsValid(false);
		}
		
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
			if(!master)master = previewBlip.getPreset().getSoundParam(p->slaveTo);
			if(!master)master = previewBlip.getPreset().getVisualParam(p->slaveTo);
			if(master)p->abs_value = ofMap(master->abs_value, master->min_val, master->max_val, p->min_val, p->max_val);
			//if(master)cout << master->name << ":"<< p->abs_value << endl;
		}
		
	
	}
	
	
}

void objectManager::endBlip(){
	
	if(previewBlip.getIsValid()){
		blips->push_back(previewBlip);
		blips->back().createDrawer(world_dims, previewBlip.getDrawer());
		blips->back().aquireIndex();
	}
	previewBlip.setIsValid(false);
	previewBlip.destroyDrawer();
	
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
	
	
	s_nodes[1] = selectNode(w_pos);

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
		
		if(previewTracks[0].getLength() < 20)recalc = true;
		
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
	
	ofVec2f q(size/t_dir);
	q.normalize();
	
	//wrapping adjustment for lengths
	if(q.x < 0 && abs(t_dir.x) > 10){
		if(size.x > 0){size.x -= world_dims.x;}else{size.x += world_dims.x;}
	}
	
	if(q.y < 0 && abs(t_dir.y) > 10){
		if(size.y > 0){size.y -= world_dims.y;}else{size.y += world_dims.y;}
	}
	
	dirs[0].set(0, size.y); 
	dirs[1].set(size.x,0);
	
	if(abs(size.x) < 10 || abs(size.y) < 10){ //if nearly aligned try a single track first
		
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


void objectManager::constrainEndPoint(ofVec2f origin, segment & s, bool isTracks, bool isNodes, bool isBlips){
	
	vector<ofVec2f> points;
	
	findCrossIntersects(origin, s.getDirection(), points);
	
	if(points.size() > 0)limitEndPointFromStart(origin, points, s);
		
}

void objectManager::constrainFromMidPoint(ofVec2f origin, segment & s, bool isTracks, bool isNodes, bool isBlips){
	
	vector<ofVec2f> points;
	
	if(isNodes)findNodeIntersects(s, points);
	if(isBlips)findParalellIntersects(s, points, false, true);
	
	if(points.size() > 0){limitStartPointFromMid(origin, points, s);}
	
	points.clear();
	
	if(isNodes)findNodeIntersects(s, points);
	if(isBlips)findParalellIntersects(s, points, false, true);
	
	if(points.size() > 0){limitEndPointFromMid(origin, points, s);}
		
}

void objectManager::repositionFromMidPoint(ofVec2f origin, segment & s, bool isTracks, bool isNodes, bool isBlips){

	vector<ofVec2f> points;
	
	if(isNodes)findNodeIntersects(s, points);
	if(isBlips)findParalellIntersects(s, points, false, true);
	
	ofVec2f old_sp(s.getStartPos());
	
	if(points.size() > 0){limitStartPointFromMid(origin, points, s);}
	
	if(old_sp == s.getStartPos()){
	
		points.clear();
		
		if(isNodes)findNodeIntersects(s, points);
		if(isBlips)findParalellIntersects(s, points, false, true);
		
		if(points.size() > 0){
			limitEndPointFromMid(origin, points, s, true);
		}
	}

	
}


//searches


bool objectManager::findCrossIntersects(ofVec2f origin, ofVec2f t_dir, vector<ofVec2f> & t_points, segment & s, bool isTracks, bool isBlips){

	t_dir /= t_dir;
	bool isIntersect = false;

	if(isTracks){
		for(int i  = 0; i < tracks->size(); i ++){
			
			if(t_dir != tracks->at(i).getDirection()){
				
				ofVec2f t_pos(origin * ofVec2f(t_dir.y,t_dir.x) + tracks->at(i).getStartPos() * t_dir);
				
				if(tracks->at(i).getInside(t_pos)){
					
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
		for(int i  = 0; i < blips->size(); i ++){
			
			if(t_dir != blips->at(i).getDirection()){
				
				ofVec2f t_pos(origin * ofVec2f(t_dir.y,t_dir.x) + blips->at(i).getStartPos() * t_dir);
				
				if(blips->at(i).getInside(t_pos)){
					
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
		for(int i = 0; i < tracks->size(); i++){ 
			
			if(s.getDirection() == tracks->at(i).getDirection()){
				
				if(tracks->at(i).getInside(s.getStartPos())||
				   tracks->at(i).getInside(s.getEndPos())||
				   s.getInside(tracks->at(i).getStartPos())||
				   s.getInside(tracks->at(i).getEndPos())
				   ){
					
					if(&t_points != &DPOINTS){
						isIntersect = true;
						t_points.push_back(tracks->at(i).getStartPos());
						t_points.push_back(tracks->at(i).getEndPos());

						
					}else{
						return true;
					}
				}
				
			}
		}
	}
	
	if(isBlips){
		for(int i = 0; i < blips->size(); i++){ 
			
			if(s.getDirection() == blips->at(i).getDirection()){
				
				if(blips->at(i).getInside(s.getStartPos())||
				   blips->at(i).getInside(s.getEndPos())||
				   s.getInside(blips->at(i).getStartPos())||
				   s.getInside(blips->at(i).getEndPos())
				   ){
				
					if(&t_points != &DPOINTS){
						isIntersect = true;
						t_points.push_back(blips->at(i).getStartPos());
						t_points.push_back(blips->at(i).getEndPos());
					}else{
						return true;
					}
				}
				
			}
		}
	}
	
	
	return isIntersect;
	
}

bool objectManager::findNodeIntersects(segment & s, vector<ofVec2f> & t_points){
	
	bool isIntersect = false;
	
	for(int i = 0; i < nodes->size(); i++){
		
		if(!nodes->at(i).getIsSelected()){
			
			if(s.getInside(nodes->at(i).getPos())){
				
				if(&t_points != &DPOINTS){
					t_points.push_back(nodes->at(i).getPos());
					isIntersect = true;
				}else{
					return true;
					
				}
				
			}else if( s.getStartPos().distanceSquared(nodes->at(i).getPos()) < 144 ||
					 s.getEndPos().distanceSquared(nodes->at(i).getPos()) < 144
					 ){
				
				if(&t_points != &DPOINTS){
					isIntersect = true;
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
	
	for(int i = 0; i < nodes->size(); i ++)nodes->at(i).setSelected(false);
	s_nodes[0] = NULL; s_nodes[1] = NULL;
	
}

void objectManager::deselectTracks(){
	
	for(int i = 0; i < tracks->size(); i ++)tracks->at(i).setSelected(false);
	s_tracks[0] = NULL; s_tracks[1] = NULL;
}

node * objectManager::selectNode(ofVec2f t_pos){
	
	node * n = NULL;
	
	for(int i = 0; i < nodes->size(); i++){
		
		if( t_pos.distanceSquared(nodes->at(i).getPos()) < 144){
			n = &nodes->at(i);
			nodes->at(i).setSelected(true);
			break;
		}
	}
	
	return n;
}

segment * objectManager::selectTrackPoint(ofVec2f t_pos){
	
	segment * t = NULL;
	
	for(int i = 0; i < tracks->size(); i++){
		if(tracks->at(i).getInside(t_pos)){
			t = &tracks->at(i);
			t->setSelected(true, t_pos);
			break;}
	}
	
	return t;
}

segment * objectManager::selectBlip(ofVec2f t_pos){
	
	segment * t = NULL;
	
	for(int i = 0; i < blips->size(); i++){
		if(blips->at(i).getInside(t_pos)){
			t = &blips->at(i);
			break;}
	}
	
	return t;
}


string objectManager::getPreviewParams(){
	
	string paramString = "";
	blipPreset p = previewBlip.getPreset();
	
	for(int i = 0; i < p.getSoundParams()->size(); i++){
		
		if(p.getSoundParam(i)->setType == PSET_USERA || p.getSoundParam(i)->setType == PSET_USERB){
			paramString = paramString + p.getSoundParams()->at(i).name;
			paramString +=  " : " + ofToString(p.getSoundParams()->at(i).abs_value, 2);
			paramString += "\n";
		}
	}
	
	return paramString;
}


//getters and setters

void objectManager::setTracks(vector<segment> * t_ref){tracks = t_ref;}
void objectManager::setNodes(vector<node> * t_ref){nodes = t_ref;}
void objectManager::setBlips(vector<blip> * t_ref){blips = t_ref;}
