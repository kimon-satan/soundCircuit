/*
 *  objectUtils.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 12/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "objectUtils.h"


float objectUtils::kTestSize = 16;

objectUtils::objectUtils(){


}

void objectUtils::limitStartPointFromMid(ofVec2f origin, vector<ofVec2f> & t_points, segment & s){
	
	vector<ofVec2f> bounds = getPlanarNeighbours(origin, t_points);
	sortNeighboursToSegment(origin, bounds, s);
	if(bounds[0] != origin){
		makeSegment(bounds[0], s.getDirection() * s.getLength(), s);
	}

}

void objectUtils::limitEndPointFromMid(ofVec2f origin, vector<ofVec2f> & t_points, segment & s, bool preserveLength ){
	
	vector<ofVec2f> bounds = getPlanarNeighbours(origin, t_points);
	sortNeighboursToSegment(origin, bounds, s);
	
	if(bounds[1] != origin){
		
		ofVec2f n_dir = (preserveLength) ? s.getDirection() * s.getLength() : bounds[1] - s.getStartPos();
		ofVec2f startPos = (preserveLength)? bounds[1] - n_dir: s.getStartPos();
		
		if(startPos.x < -world_dims.x/2)startPos.x += world_dims.x;
		if(startPos.y < -world_dims.y/2)startPos.y += world_dims.y;
		
		if(
		   (s.getTestArea().inside(origin) && !s.getTestArea().inside(bounds[1]))
		   ||(!s.getTestArea().inside(origin) && s.getTestArea().inside(bounds[1]))
		   ||(s.getWrapTestArea().inside(origin) && s.getWrapTestArea().inside(bounds[1]))
		   ){
			n_dir += world_dims * s.getDirection();
		}
		
		makeSegment(startPos, n_dir, s);
	}
}

void objectUtils::limitEndPointFromStart(ofVec2f origin, vector<ofVec2f> & t_points, segment & s){
	
	bool spIsStart = (s.getStartPos() == origin)? true : false; 
	
	//create a dummy bound at the start of the track
	ofVec2f dummy_point = origin + ofVec2f((spIsStart)? s.getDirection() * WORLD_UNIT: -s.getDirection()) * WORLD_UNIT;
	t_points.push_back(dummy_point);
	
	//make an origin inside the track
	ofVec2f n_origin = origin + ofVec2f((spIsStart)? s.getDirection() * WORLD_UNIT * 2 : -s.getDirection() * WORLD_UNIT * 2);
	
	vector<ofVec2f> bounds = getPlanarNeighbours(n_origin, t_points);
	sortNeighboursToSegment(n_origin, bounds, s);
	
	ofVec2f ep = (spIsStart) ? bounds[1] : bounds[0];
	
	bool isWrapped = false;
	
	if(s.getIsWrapped()){
		if((s.getTestArea().inside(n_origin) && s.getWrapTestArea().inside(ep))
		   ||(s.getWrapTestArea().inside(n_origin) && s.getTestArea().inside(ep))){
			isWrapped = true;
		}
	}
	
	if(ep != n_origin){
		
		ofVec2f n_dir = (spIsStart) ? ep - origin :  -(origin - ep);
		
		if(isWrapped){
			if(n_dir.x + n_dir.y > 0){
				n_dir = world_dims * s.getDirection() - n_dir;
				n_dir *= -1;
			}else{
				n_dir += world_dims * s.getDirection();
			}
		}
		
		makeSegment(origin, n_dir, s);
	}
}


vector<ofVec2f> objectUtils::getPlanarNeighbours(ofVec2f origin, vector<ofVec2f> &t_points){
	
	vector<ofVec2f> finalPoints;
	
	if(t_points.size() < 1){
		for(int i = 0; i < 2; i++)finalPoints.push_back(origin);
		return finalPoints;
	}
	
	ofVec2f t_dir(t_points[0] - origin);
	t_dir /= t_dir;
	
	ofVec2f startPos(origin);
	ofVec2f wrapStartPos(origin);
	ofVec2f endPos(origin);
	ofVec2f wrapEndPos(origin);
	
	float dist_s = world_dims.length();
	float dist_e = world_dims.length();
	
	float o_local = origin.x * t_dir.x + origin.y * t_dir.y;
	
	for(int i = 0; i < t_points.size(); i ++){
		
		float t_dist = t_points[i].distance(origin);
		float t_local = t_points[i].x * t_dir.x + t_points[i].y * t_dir.y;
		float ws_local = wrapStartPos.x * t_dir.x + wrapStartPos.y * t_dir.y;
		float we_local = wrapEndPos.x * t_dir.x + wrapEndPos.y * t_dir.y;   
		
		if(t_local < o_local && 
		   t_dist < dist_s){
			startPos = t_points[i];
			dist_s = t_dist;
		}
		if(t_local > o_local &&
		   t_dist < dist_e
		   ){
			endPos = t_points[i];
			dist_e = t_dist;
		}
		
		if(t_local > ws_local)wrapStartPos = t_points[i];
		if(t_local < we_local)wrapEndPos = t_points[i];
		
	}
	
	if(startPos != origin){
		finalPoints.push_back(startPos);
	}else {
		finalPoints.push_back(wrapStartPos);
	}
	
	if(endPos != origin){
		finalPoints.push_back(endPos);
	}else {
		finalPoints.push_back(wrapEndPos);
	}
	
	return finalPoints;
	
}

void objectUtils::sortNeighboursToSegment(ofVec2f origin, vector<ofVec2f> &t_points, segment & s){
	
	ofVec2f norm_dir = s.getDirection()/s.getDirection();
	
	for(int i = 0; i < 2; i++){
		
		ofVec2f t_bound(t_points[i]);
		
		if((s.getTestArea().inside(origin) && !s.getTestArea().inside(t_points[i]))||
		   (s.getIsWrapped() && s.getWrapTestArea().inside(origin) && !s.getWrapTestArea().inside(t_points[i]))
		   ){
			
			if(s.getTestArea().inside(origin)){
				t_bound += world_dims * norm_dir;
			}else{
				t_bound -= world_dims * norm_dir;
			}
			
		}
		
		if(i == 0){
			if(ofVec2f(origin - t_bound).normalize() != s.getDirection()){
				t_points[0] = origin;
			}
		}else{
			if(ofVec2f(t_bound - origin).normalize() != s.getDirection()){
				t_points[1] = origin;
			}
		}
		
		if(!s.getInside(t_points[i])){t_points[i] = origin;}
		
	}
	
	
}

void objectUtils::makeSegment(ofVec2f w_pos, ofVec2f t_dir, segment & s){
	
	s.setStartPos(w_pos);
	float t_length = t_dir.length();
	quantizeDirection(t_dir);
	
	ofVec2f b_pos(w_pos + t_dir * t_length);
	
	if(t_dir.x + t_dir.y < 0){
		s.setStartPos(b_pos);
		t_dir *= -1;
	}
	
	s.setLength(t_length);
	s.setDirection(t_dir);
	updateTestAreas(s);
	
}

void objectUtils::updateTestAreas(segment & t){
	
	//track direction must always run left to right or top to bottom
	
	ofRectangle world;
	world.setFromCenter(0,0,world_dims.x,world_dims.y);
	
	ofVec2f p = t.getStartPos() + t.getDirection() * t.getLength();
	
	if(world.inside(p) && world.inside(t.getStartPos())){ //for no wrap
		
		ofVec2f tlc, dims;
		t.setIsWrapped(false);
		
		if(t.getDirection().x > 0){
			tlc.set(t.getStartPos().x + WORLD_UNIT,t.getStartPos().y -WORLD_UNIT * kTestSize/2);
			dims.set(t.getLength() - WORLD_UNIT * 2, WORLD_UNIT * kTestSize);
		}else{
			tlc.set(t.getStartPos().x - WORLD_UNIT * kTestSize/2, t.getStartPos().y + WORLD_UNIT * 2);
			dims.set(WORLD_UNIT * kTestSize, t.getLength() - WORLD_UNIT * 2);
		}
		
		t.setTestArea(ofRectangle(tlc.x, tlc.y, dims.x, dims.y));
		t.setEndPos(p);
		
	}else{ //nb test and wrap areas overlap each other
		
		ofVec2f tlc, dims, w_tlc, w_dims, wp, wsp;
		t.setIsWrapped(true);
		
		if(t.getDirection().x > 0){
			
			if(!world.inside(p)){
				wp.set(p.x - world_dims.x, p.y); //wrap p
				wsp.set(t.getStartPos());
			}else{
				wp.set(p); 
				wsp.set(t.getStartPos().x + world_dims.x, t.getStartPos().y);//wrap sp
			}
			
			ofVec2f left_i(-world_dims.x/2, wsp.y);
			ofVec2f right_i(world_dims.x/2, wsp.y);
			
			dims.set((right_i - wsp).length() + WORLD_UNIT,WORLD_UNIT * kTestSize);
			w_dims.set((wp - left_i).length() ,WORLD_UNIT * kTestSize);
			tlc.set(wsp.x + WORLD_UNIT, wsp.y -WORLD_UNIT * kTestSize/2);
			w_tlc.set(left_i.x - WORLD_UNIT,left_i.y -WORLD_UNIT * kTestSize/2);
			
			
		}else{
			
			if(!world.inside(p)){
				wp.set(p.x, p.y - world_dims.y); //wrap p
				wsp.set(t.getStartPos());
			}else{
				wp.set(p); 
				wsp.set(t.getStartPos().x,t.getStartPos().y + world_dims.y);//wrap sp
			}
			
			ofVec2f top_i(t.getStartPos().x, -world_dims.y/2);
			ofVec2f bottom_i(t.getStartPos().x, world_dims.y/2);
			
			dims.set(WORLD_UNIT * kTestSize,(bottom_i - wsp).length()+WORLD_UNIT);
			w_dims.set(WORLD_UNIT * kTestSize,(wp - top_i).length());
			
			tlc.set(wsp.x - WORLD_UNIT * kTestSize/2, wsp.y + WORLD_UNIT);
			w_tlc.set(top_i.x - WORLD_UNIT * kTestSize/2,top_i.y - WORLD_UNIT);
			
			
		}
		
		t.setTestArea(ofRectangle(tlc.x, tlc.y,dims.x, dims.y));
		t.setWrapTestArea(ofRectangle(w_tlc.x, w_tlc.y,w_dims.x, w_dims.y));
		t.setEndPos(wp);
		t.setStartPos(wsp); //incase it needs wrapping
		
	}
	
	
}

void objectUtils::quantizeDirection(ofVec2f & t_dir){
	
	t_dir.normalize();
	int angle = t_dir.angle(ofVec2f(0,-1));
	angle = angle/45;
	
	switch (angle) {
		case 0:t_dir.set(0,-1);break;
		case 1:case 2:t_dir.set(-1,0);break;
		case -1:case -2:t_dir.set(1,0);break;
		case 3:case -3:t_dir.set(0,1);break;
	}
	
}

//getters and setters

void objectUtils::setWorldDims(ofVec2f t_dims){world_dims = t_dims;}
ofVec2f objectUtils::getWorldDims()const{return world_dims;}
