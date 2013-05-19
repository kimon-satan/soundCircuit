/*
 *  layer.cpp
 *  soundCircuit
 *
 *  Created by Simon Katan on 04/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "layer.h"



layer::layer():objectRenderer(){

	isScreenData = false;
	
}

/*layer::layer(const layer &src):objectRenderer(){ //only one layer so no need for this atm
	
	mReader.setLayer(this);
	world_dims = src.getWorldDims();
	incr = mReader.getIncrement();
	isScreenData = false;
	
	blips = src.getBlips();
	nodes = src.getNodes();
	tracks = src.getTracks();
	
	mReader = src.getReader();
	mReader.setLayer(this);
	

	isScreenData = false;

}*/

void layer::update(){
	
	for(vector<reader>::iterator it = mReaders.begin(); it != mReaders.end(); it++)it->update();
	for(vector<blip>::iterator it = blips.begin(); it != blips.end(); it++)it->update();
    updateSelected();
	
}

void layer::drawDebug(ofColor col, ofVec2f mouse){
	
    
    
    if(isScreenData){
        ofSetColor(col);
        ofFill();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofRect(0,0,world_dims.x, world_dims.y);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofNoFill();
        ofSetColor(0);
 
         glDepthFunc(GL_ALWAYS);
         ofSetColor(0);
         ofNoFill();
         ofCircle(mouse.x, mouse.y, 5);
         ofDrawBitmapString(ofToString(mouse.x,1) + ", " + ofToString(mouse.y,1), mouse.x, mouse.y);
         glDepthFunc(GL_LESS);
 
     }
    
	

}

void layer::drawReaders(){for(int i = 0; i < mReaders.size(); i ++)mReaders[i].draw();}


e_objectType layer::selectSomething(ofVec2f w_pos, bool isReader, bool isBlip, bool isNode, bool isTrack){

	deselectAll();
	
	if(isReader){
		reader * rd_ptr = getNearestReader(w_pos);
		if(rd_ptr)rd_ptr->setIsSelected(true);	
		return OT_READER;
	}
	
	//if(isBlip){
	//	if(selectBlip(w_pos))return OT_BLIP;
	//}
	
	if(isNode){
		if(selectNode(w_pos, kTestSize/2))return OT_NODE;
	}
	
	if(isTrack){
		if(selectTrackPoint(w_pos))return OT_TRACK;
	}
		
	return OT_WORLD;

}

void layer::deselectAll(){

	deselectNodes();
	deselectTracks();
	deselectReaders();
}

reader * layer::selectReader(ofVec2f w_pos){

	for(int i = 0; i < mReaders.size(); i ++){
		if(mReaders[i].getInside(w_pos)){
			return &mReaders[i];
		}
	}
	
	return NULL;
}


reader * layer::getNearestReader(ofVec2f w_pos){

	float dist = world_dims.x * world_dims.y;
	reader * r_ptr = NULL;
	
	for(int i = 0; i < mReaders.size(); i ++){
	
		float td = mReaders[i].getPos().distance(w_pos);
		if(td < dist){
			dist = td;
			r_ptr = &mReaders[i];
		}
	}
	
	return r_ptr;

}

void layer::deselectReaders(){

	for(int i = 0; i < mReaders.size(); i ++)mReaders[i].setIsSelected(false);
	
}




void layer::destroyReader(reader * r_ptr){

	if(r_ptr){
		int t_index = r_ptr->getIndex();
		r_ptr->endCurrentBlips();
		vector<reader>::iterator it = remove_if(mReaders.begin(), mReaders.end(),bind2nd(readerIndex(), t_index));
		mReaders.erase(it, mReaders.end());
	}
	
}


void layer::beginInsertSpace(ofVec2f t_pos, ofVec2f t_dir){
	
	float initSpace = kTestSize/2;
	
	objectManager::beginInsertSpace(t_pos, t_dir, initSpace);
	for(int i = 0; i < mReaders.size(); i ++){mReaders[i].beginInsertion(t_pos, t_dir, initSpace);}
	
	for(int i = 0; i < mReaders.size(); i ++)if(!selectTrackPoint(mReaders[i].getPos()))destroyReader(&mReaders[i]);
	deselectAll();
	
}

void layer::beginInsertion(ofVec2f t_pos, ofVec2f t_dir){

	for(int i = 0; i < mReaders.size(); i ++)mReaders[i].beginInsertion(t_pos, t_dir,0);
	objectManager::beginInsertion(t_pos, t_dir);
	
}

void layer::resizeInsertion(float size){
	
	for(int i = 0; i < mReaders.size(); i ++)mReaders[i].resizeInsertion(size);
	objectManager::resizeInsertion(size);

}

void layer::endInsertion(){
	
	objectManager::endInsertion();
	
}


reader* layer::addReader(ofVec2f w_pos){

	deselectAll();
	s_tracks[0] = selectTrackPoint(w_pos);
	
	if(s_tracks[0]){
		reader r;
		r.setOscSender(sender);
		r.aquireIndex();
		r.setLayer(this);
		r.setPos(s_tracks[0]->getSelectPos());
		r.setDirection(s_tracks[0]->getDirection());
		mReaders.push_back(r);
		deselectAll();
		return &mReaders.back();
	}
	
	return NULL;
}


void layer::toggleScreenData(){isScreenData = !isScreenData;}

//getters and setters


vector<node> * layer::getNodesRef(){return &nodes;}
vector<blip> * layer::getBlipsRef(){return &blips;}

vector<node> layer::getNodes()const{return nodes;}
vector<blip> layer::getBlips()const{return blips;}
vector<segment> layer::getTracks()const{return tracks;}

reader * layer::getReaderRef(int i){

	vector<reader>::iterator it = find_if(mReaders.begin(), mReaders.end(), bind2nd(readerIndex(), i));
	return &(*it);
}
reader layer::getReader(int i){

	vector<reader>::iterator it = find_if(mReaders.begin(), mReaders.end(), bind2nd(readerIndex(), i));
	return *it;
}

void layer::setOSC(ofxOscSender * s){sender = s;}

