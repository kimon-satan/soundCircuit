#pragma once

#include "ofMain.h"
#include "layer.h"
#include "ofxOsc.h"
#include "ofxRotRect.h"
#include "ofxGrabCam.h"


#define HOST "localhost"
#define PORT 57120



//--------------------------------------------------------
class testApp : public ofBaseApp{
	
public:
	
	testApp();
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
	void loadTimeAttributes(ofxXmlSettings XML, blipPreset & bp);
	
	ofVec2f moduloPoint(ofVec2f t_point, ofVec2f t_dims);
	ofVec3f moduloPoint(ofVec3f t_point, ofVec2f t_dims);
	void prepPauseFollow();
	
	void startAction();
	void continueAction();
	void endAction();
	
	void addLayer();
	
	enum e_mouseMode {
		MODE_NONE,
		MODE_DRAG,
		MODE_ADD_BLIP,
		MODE_ADD_TRACK,
		MODE_INSERT_SPACE,
		MODE_COUNT
	};
	
	enum e_Action {
		ACTION_NONE,
		ACTION_DRAG,
		ACTION_ADD_SHORT_TRACK,
		ACTION_ADD_LONG_TRACK,
		ACTION_ADD_BLIP,
		ACTION_INSERT_SPACE,
		ACTION_COUNT
	};
	

	int lagCount;
	const float kLagFrames;
	const float kMinIncr;
	ofxGrabCam cam;
	ofRectangle viewPort;
	
	string getModeString(e_mouseMode temp);
	
	layer world;
	
	ofVec2f mouseP, mouseP_prev, mouseP_down;
	ofVec3f mouseW;
	ofVec3f mousePick, mousePick_down;
	
	vector<ofVec3f> VPW_coords;
	vector<ofVec2f> VP_coords;
	ofRectangle roi;
	vector<ofColor> testCols;
	
	float rots[3];
	float targetRots[3];
	
	vector<vector<blipPreset> > presets;
	
	int selectedPreset[2];
	
	ofxOscSender sender;
	reader * currentReader;
	
	bool isFixed, pauseFollow;
	ofVec2f trans;
	ofVec2f direction;
	
	e_mouseMode mouseMode;
	e_Action currentAction;
	int buttonMode;
	bool isOptionKey, isMouseDown;
	bool isPreview;
	static bool drawData;

	
};

