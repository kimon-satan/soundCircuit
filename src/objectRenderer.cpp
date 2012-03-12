/*
 *  objectRenderer.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 05/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "objectRenderer.h"

objectRenderer::objectRenderer():objectManager(){

	isTrackData = false;
	isNodeData = false;
	isBlipData = false;

}

void objectRenderer::render(ofVec2f t_pos, ofRectangle roi){
	
	viewPort = roi;
	center.set(t_pos);
	
	//adjust the viewport for which tile is being drawn
	viewPort.x -= center.x;
	viewPort.y -= center.y;
	
	drawTracks();
	drawNodes();
	drawBlips();
	drawSelected();
	
	
}


bool objectRenderer::checkIsVisible(ofVec2f t_pos){
	
	return(viewPort.inside(t_pos));
	
}

bool objectRenderer::checkIsVisible(ofVec2f a, ofVec2f b, ofVec2f t_dir){
	
	
	if(t_dir.x > 0){
		
		if(viewPort.inside(viewPort.x + viewPort.width/2, a.y)){
			if(a.x < viewPort.x + viewPort.width && b.x > viewPort.x){
				return true;
			}
		}
		
	}else if(t_dir.y > 0){
		
		if(viewPort.inside(a.x, viewPort.y + viewPort.height/2)){
			if(a.y < viewPort.y + viewPort.height && b.y > viewPort.y)return true;
		}
		
	}
	
	return false; 
}


bool objectRenderer::checkIsVisible(vector<ofVec2f> t_corners){
	
	vector<ofVec2f> points;
	points.push_back(t_corners[0]);
	points.push_back(t_corners[1]);
	points.push_back(ofVec2f(t_corners[0].x, t_corners[1].y));
	points.push_back(ofVec2f(t_corners[1].x, t_corners[0].y));
	
	for(int i = 0; i < points.size(); i++){
		points[i].x = min(points[i].x, world_dims.x);
		points[i].x = max(points[i].x, -world_dims.x);
		points[i].y = min(points[i].y, world_dims.y);
		points[i].y = max(points[i].y, -world_dims.y);
		if(viewPort.inside(points[i]))return true;
		
	}
	
	return false; 
}

void objectRenderer::drawTracks(){
	
	for(int i = 0; i < tracks.size(); i ++){
		
		drawTrack(&tracks.at(i));
	}
	
	if(isPreview){
		
		for(int i = 0; i <2; i++){
			if(previewTracks[i].getIsValid())drawTrack(&previewTracks[i]);
		}
	}

}

void objectRenderer::drawTrack(segment * t){
	
	glPushMatrix();
	glTranslatef(0, 0, -WORLD_UNIT/2.0f);
	ofVec2f sp(t->getStartPos());
	ofVec2f lp;
	
	if(t->getDirection().x > 0){
		lp.set(t->getTestArea().x + t->getTestArea().width + WORLD_UNIT * 2, sp.y);
	}else{
		lp.set(sp.x, t->getTestArea().y + t->getTestArea().height + WORLD_UNIT * 2);
	}
	
	
	if(checkIsVisible(sp, lp, t->getDirection())){
		ofSetColor(100);
		ofLine(sp.x,sp.y,lp.x,lp.y);
	}
	
	if(t->getIsWrapped()){
		
		ofVec2f wsp;
		ofVec2f wlp;
		
		if(t->getDirection().x > 0){
			wsp.set(t->getWrapTestArea().x - WORLD_UNIT, sp.y);
			wlp.set(t->getWrapTestArea().x + t->getWrapTestArea().width + WORLD_UNIT, sp.y);
		}else{
			wsp.set(sp.x,t->getWrapTestArea().y - WORLD_UNIT);
			wlp.set(sp.x, t->getWrapTestArea().y + t->getWrapTestArea().height + WORLD_UNIT);
		}
		
		if(checkIsVisible(wsp, wlp, t->getDirection())){
			ofSetColor(100);
			ofLine(wsp.x, wsp.y, wlp.x, wlp.y);
		}
		
		
	}
		
	if(t->getIsSelected())selected.push_back(t->getSelectPos());

	
	if(isTrackData){
		ofDrawBitmapString("sp", sp);
		ofNoFill();
		ofSetColor(255, 0, 0);
		ofRect(t->getTestArea());
		
		if(t->getIsWrapped()){
			ofSetColor(0, 0, 255);
			ofRect(t->getWrapTestArea());
		}
	}
	
	glPopMatrix();


}

void objectRenderer::drawSelected(){

	for(int i = 0; i < selected.size(); i++){
		
		ofPushMatrix();
		glDepthFunc(GL_ALWAYS);
		ofEnableAlphaBlending();
		ofSetColor(255, 0, 0, 100);
		ofFill();
		ofCircle(selected[i], kTestSize * WORLD_UNIT/2);
		ofDisableAlphaBlending();
		glDepthFunc(GL_LESS);
		ofPopMatrix();

	}
	
	selected.clear();

}


void objectRenderer::drawNodes(){
	
	for(vector<node>::iterator it = nodes.begin(); it != nodes.end(); it++){
		
		ofVec2f pos(it->getPos());
		
		if(checkIsVisible(pos)){
			
			if(it->getIsShown()){
				
				float radius = kTestSize * (float)WORLD_UNIT/2.0f;
				
				glPushMatrix();
				glTranslatef(pos.x, pos.y, -WORLD_UNIT);
				if(it->getIsAdjusting())glScalef(3, 3, 0);
				glDepthFunc(GL_LEQUAL);
				ofFill();
				ofSetColor(255);
				ofCircle(0,0, kTestSize * (float)WORLD_UNIT/2.0f);
				
				ofEnableSmoothing();
				
				ofSetColor(180);
				vector<bool> sockets = it->getNowSockets();
				float innerRad = radius - (WORLD_UNIT * 3);
				
				for(int i = 0; i < 4; i ++){
					if(!sockets[i]){
						
						ofBeginShape();
						ofVertex(0,0);
						ofVec2f o_pos(-innerRad, -innerRad);
						o_pos.rotate(90 * i);
						ofVertex(o_pos.x, o_pos.y);
						
						for(int i = 0; i < 30; i ++){
							o_pos.rotate(3);
							ofVertex(o_pos.x, o_pos.y);
						}
						
						ofVertex(0,0);
						ofEndShape(true);
						
					}
				}
			
				
				ofSetColor(100);
				ofNoFill();
				ofCircle(0,0, radius);
				ofLine( - innerRad,  - innerRad, innerRad, innerRad);
				ofLine( innerRad,  - innerRad, - innerRad, innerRad);
				
				glDepthFunc(GL_LESS);
				ofDisableSmoothing();
				
				glPopMatrix();
				
			}
		
			
			if(it->getIsSelected() && !it->getIsAdjusting())selected.push_back(it->getPos());
			
		}
		
		
		if(isNodeData){
			ofNoFill();
			ofSetColor(0, 0, 255);
			ofCircle(pos, kTestSize * WORLD_UNIT/2);
			ofDrawBitmapString(ofToString(it->getIndex()), pos - 0.03);
		}
		
	}
	

	
}

void objectRenderer::drawBlips(){

	glPushMatrix();
	
	glTranslatef(0, 0, -WORLD_UNIT); //for z buffering
	
	for(vector<blip>::iterator it = blips.begin(); it != blips.end(); it++){
		
		if(checkIsVisible(it->getDrawer()->getCorners())){
			it->draw(0);
		}
		
		if(isBlipData){
			ofSetColor(255,0,0);
			ofDrawBitmapString("sp", it->getStartPos());
			ofNoFill();
			ofSetColor(0, 100, 255);
			ofRect(it->getTestArea());
			
			if(it->getIsWrapped()){
				ofSetColor(0, 255, 0);
				ofRect(it->getWrapTestArea());
			}
		}
		

		
	}
	
	if(previewBlip.getIsValid()){
	
	
		if(previewBlip.getDrawer()){
			
			previewBlip.draw(0);
		
		}
	
		if(isBlipData){
			ofSetColor(255,0,0);
			ofDrawBitmapString("sp", previewBlip.getStartPos());
			ofNoFill();
			ofSetColor(0, 100, 255);
			ofRect(previewBlip.getTestArea());
			
			if(previewBlip.getIsWrapped()){
				ofSetColor(0, 255, 0);
				ofRect(previewBlip.getWrapTestArea());
			}
		}
		

		
	}
	
	glPopMatrix();

}



//getters and setters

void objectRenderer::toggleTrackData(){isTrackData = !isTrackData;}
void objectRenderer::toggleNodeData(){isNodeData = !isNodeData;}
void objectRenderer::toggleBlipData(){isBlipData = !isBlipData;}