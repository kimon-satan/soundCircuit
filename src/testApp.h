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
	
	void setupDummyPresets();
	
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
	
	layer currentLayer;
	
	
	void updateDummyViews();
	ofVec2f getWorldCoordinate(ofVec2f point);
	void drawVp(ofRectangle& vp, bool isDummy = false);
	void moduloViewPort();
	void prepPauseFollow();

	enum e_mouseMode {
		MODE_NONE,
		MODE_DRAG,
		MODE_ADD_TRACK,
		MODE_ADD_BLIP,
		MODE_HYBRID,
		MODE_COUNT
	};
	
	string getModeString(e_mouseMode temp);
	
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
	bool isOptionKey;
	bool isPreview, mouseDown;
	static bool drawData;

	
};

