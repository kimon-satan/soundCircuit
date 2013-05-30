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

struct socket{

    int index;
    bool left;
    bool right;
    bool straight;
    ofVec2f dir;
    bool available;
    
    socket(int i, ofVec2f d, bool a, bool l, bool s, bool r){
        
        index = i;
        left = l;
        straight = s;
        right = r;
        dir = d;
        available = a;
    }
    

};

enum e_sockMode{
  
    SM_ALL,
    SM_LEFT,
    SM_RIGHT,
    SM_STRAIGHT

};


class node{

public:
	
	node(ofVec2f t_pos);
	
	void addSocket(ofVec2f t_dir);
	void removeSocket(ofVec2f t_dir);
	void openSocket(ofVec2f t_dir, int mode = 0);
	void closeSocket(ofVec2f t_dir, int mode = 0);
	void openSocket(int t, int mode = 0);
	void closeSocket(int t, int mode = 0);
	
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
	
	
	vector<bool> getNowSockets();
	bool getNowSocket(ofVec2f t_dir);
	bool getNowSocket(int i);
    
    vector<socket> getNowSockets_n();
	socket getNowSocket_n(ofVec2f t_dir);
	socket getNowSocket_n(int i);
    
    
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
    
    vector<socket> nowSockets_n;
    
	bool isSelected, isShown, isAdjusting;
	int numSockets, numOpen;
	
	int index;
	static int tCounter;
	
	
};

#endif
