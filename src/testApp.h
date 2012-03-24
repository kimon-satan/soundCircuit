#pragma once

#include "ofMain.h"
#include "layer.h"
#include "ofxOsc.h"
#include "ofxGrabCam.h"
#include "bank.h"

#define HOST "localhost"
#define PORT 57120




//--------------------------------------------------------
class testApp : public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);		
	void exit();

	
private:
	
	void loadPresets(ofxXmlSettings XML);
	void loadBanks(ofxXmlSettings XML);
	void loadParamAttribute(ofxXmlSettings XML, paramAttributes * p);
	void loadTimeAttributes(ofxXmlSettings XML, blipPreset & bp);
	
	
	void startAction();
	void continueAction();
	void endAction();
	
	enum e_mouseMode {
		MODE_ADD,
		MODE_ADJUST,
		MODE_DESTROY,
		MODE_COUNT
	};
	
	enum e_Action {
		ACTION_NONE,
		ACTION_DRAG,
		ACTION_ADD_SHORT_TRACK,
		ACTION_ADD_LONG_TRACK,
		ACTION_ADD_BLIP,
		ACTION_ADJUST_BLIP,
		ACTION_DESTROY_BLIP,
		ACTION_INSERT_SPACE,
		ACTION_STRETCH_SPACE,
		ACTION_ADJUST_NODE,
		ACTION_ADD_READER,
		ACTION_ADJUST_READER,
		ACTION_DESTROY_READER,
		ACTION_FOLLOW_READER,
		ACTION_COUNT
	};
	
	
	ofxGrabCam cam;
	ofRectangle viewPort;
	
	string getModeString(e_mouseMode temp);
	
	layer world;
	
	ofVec2f mouseP, mouseP_prev, mouseP_down;
	ofVec3f mouseW;
	ofVec3f mousePick, mousePick_down;
	int mouseDownCount;
	
	vector<ofVec3f> VPW_coords;
	vector<ofVec2f> VP_coords;
	ofRectangle roi;
	vector<ofColor> testCols;
	
	vector<bank> banks;
	
	int currentBank;
	
	
	ofxOscSender sender;
	reader * currentReader;
	reader * oldReader;
	
	e_mouseMode mouseMode;
	e_mouseMode overrideMode;
	bool isReader, isTrack;
	
	e_objectType currentSelection;
	e_Action currentAction;
	int buttonMode;
	bool isOptionKey, isMouseDown;
	bool isPreview;
	static bool drawData;

	
};

