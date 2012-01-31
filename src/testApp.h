#pragma once

#include "ofMain.h"
#include "layer.h"
#include "reader.h"
#include "ofxOsc.h"


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
	
	void loadPresets();
	void loadParamAttribute(ofxXmlSettings XML, paramAttributes * p);
	void updateDummyViews();
	ofVec2f getWorldCoordinate(ofVec2f point);
	void drawVp(ofRectangle& vp, bool isDummy = false);
	void moduloViewPort();
	void prepPauseFollow();
	
	void startAction();
	void continueAction(ofVec2f dir);
	void endAction();

	enum e_mouseMode {
		MODE_NONE,
		MODE_DRAG,
		MODE_ADD,
		MODE_COUNT
	};
	
	enum e_Action {
		ACTION_NONE,
		ACTION_DRAG,
		ACTION_ADD_SHORT_TRACK,
		ACTION_ADD_LONG_TRACK,
		ACTION_ADD_BLIP,
		ACTION_COUNT
	};
	
	string getModeString(e_mouseMode temp);
	
	layer currentLayer;
	ofVec2f mouse_offset;
	ofRectangle viewPort;
	ofVec2f world_dims;
	ofVec2f mouse_a, mouse_b;
	
	vector<ofVec2f> vpHist;
	vector<ofRectangle> dummy_views;
	vector<blipPreset> presets;
	
	int selectedPreset;
	
	ofxOscSender sender;
	reader thisReader;
	bool isFixed, pauseFollow;
	ofVec2f trans;
	
	e_mouseMode mouseMode;
	e_Action currentAction;
	bool isOptionKey;
	bool isPreview, mouseDown;
	static bool drawData;

	
};

