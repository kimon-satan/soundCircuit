/*
 *  node.h
 *  soundCircuit
 *
 *  Created by Simon Katan on 02/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef node_h
#define node_h

#include "ofMain.h"
//class reader;


class node{

public:
	
	node(ofVec2f t_pos);
	
	void addSocket(ofVec2f t_dir);
	void removeSocket(ofVec2f t_dir);
	void openSocket(ofVec2f t_dir);
	void closeSocket(ofVec2f t_dir);
	void openSocket(int t);
	void closeSocket(int t);
	
	static int getSocketIndex(ofVec2f t_dir);
	static ofVec2f getSocketDirection(int i);
	
	bool getSuperfluous();
	int getIndex();
	void aquireIndex();
	
	
	//getters and setters
	
	int getNumSockets();
	ofVec2f getPos();
	void setPos(ofVec2f p);
	void setSelected(bool t);
	bool getIsSelected();
	
	/*void addReader(reader * r);
	bool getReaderPresent(reader * r);
	void removeReader(reader * r);*/
	
	vector<bool> getNowSockets();
	bool getNowSocket(ofVec2f t_dir);
	bool getNowSocket(int i);
	vector<bool> getAllSockets();
	bool getAllSocket(ofVec2f t_dir);
	bool getIsShown();
	bool getIsAdjusting();
	void setIsAdjusting(bool t);

private:
	
	ofVec2f pos;
	vector<bool> nowSockets;
	static ofVec2f socketDirections[4];
	vector<bool> allSockets;
	bool isSelected, isShown, isAdjusting;
	int numSockets, numOpen;
	//vector<reader*> readerPtrs;
	
	int index;
	static int tCounter;

	
	
	
};

#endif
