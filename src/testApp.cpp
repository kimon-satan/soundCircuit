#include "testApp.h"

bool testApp::drawData = false;

testApp::testApp(){


}

//--------------------------------------------------------------
void testApp::setup(){
	
	//basics
	
	sender.setup( HOST, PORT );
	ofxOscMessage m;
	m.setAddress("/init");
	sender.sendMessage(m);
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	
	//viewPort variables
	
	viewPort.set(0,0,ofGetScreenWidth(),ofGetScreenHeight());
	isPreview = false;
	
	for(int i =0; i < 4; i++)VPW_coords.push_back(ofVec3f(0,0,0));
	VP_coords.push_back(ofVec2f(1,1));
	VP_coords.push_back(ofVec2f(ofGetScreenWidth()-1,0));
	VP_coords.push_back(ofVec2f(0,ofGetScreenHeight()));
	VP_coords.push_back(ofVec2f(ofGetScreenWidth(),ofGetScreenHeight()));
	
	for(int i = 0; i < 3; i ++){
		for(int j = 0; j < 3; j++){
			ofColor t;
			t.setHsb((i * 3 + j) * 230/9, 100, 255, 255);
			testCols.push_back(t);
		}
	}
	

	//interface stuff
	
	isMouseDown = false;
	isOptionKey = false;
	mouseMode = MODE_NONE;
	currentAction = ACTION_NONE;
	buttonMode = 0;
	
	//preset stuff
	
	blipPreset::thisSynthDef.loadDictionary();
	
	for(int i =0 ; i < 4; i++){
		vector<blipPreset> t;
		presets.push_back(t);
	}
	
	loadPresets();
	selectedPreset[0] = 0;
	selectedPreset[1] = 0;
	
	//the world
	
	world.setWorldDims(ofVec2f(1500,1500));
	currentReader = world.getReaderRef();
	currentReader->setOscSender(&sender);
	world.beginTrack(ofVec2f(0,0));
	world.calcTrack(ofVec2f(1,0),ofVec2f(1,0), 1);
	world.endTrack();
	
	
}

void testApp::loadPresets(){
	
	ofxXmlSettings XML;
	
	if(XML.loadFile("presets.xml")){
		
		if(XML.pushTag("SOUND_CIRCUIT", 0)){
			
			if(XML.pushTag("BLIP_PRESETS", 0)){
				
				int numBlips = XML.getNumTags("BLIP");
				
				for(int i = 0; i < numBlips; i++){
					
					if(XML.pushTag("BLIP", i)){
						
						blipPreset t_blip[4];
						
						for(int j = 0; j < 4; j++){
							t_blip[j].setName(XML.getValue("NAME", ""));
							t_blip[j].setSynthDef(XML.getValue("SYNTH", ""));
							t_blip[j].setDrawType(XML.getValue("DRAW", ""));
							t_blip[j].setEnvType(e_envType(XML.getValue("ENV", ENV_ASR)));
						}
						
						if(XML.pushTag("LENGTH", 0)){
							
							int numAlt = XML.getNumTags("ALT");
							
							for(int j = 0; j < 4; j++){
								paramAttributes * p = t_blip[j].getLength();
								
								if(j > 0 && j <= numAlt){
									if(XML.pushTag("ALT", j-1)){
										loadParamAttribute(XML, p);
										XML.popTag();
									}
								}else{
									loadParamAttribute(XML, p);
								}
							}
					
							XML.popTag();
						}
						
						if(XML.pushTag("ATTACK", 0)){ //continue updating from here ... needs a more efficient way of coding
							
							int numAlt = XML.getNumTags("ALT");
							
							for(int j = 0; j < 4; j++){
								
								if(j > 0 && j <= numAlt)XML.pushTag("ALT", j-1);
								
								if(XML.pushTag("PROP", 0)){
									t_blip[j].setIsAttackProp(true);
									paramAttributes * p = t_blip[j].getAttackProp();
									loadParamAttribute(XML, p);
									XML.popTag();
								}else{
									paramAttributes * p = t_blip[j].getAttackSecs();
									loadParamAttribute(XML, p);
									t_blip[j].setIsAttackProp(false);
								}
								
								if(j > 0 && j <= numAlt)XML.popTag();
							}
							

							XML.popTag();
						}
						
						if(XML.pushTag("DECAY", 0)){
							
							int numAlt = XML.getNumTags("ALT");
							
							for(int j = 0; j < 4; j++){
								
								if(j > 0 && j <= numAlt)XML.pushTag("ALT", j-1);
								
								if(XML.pushTag("PROP", 0)){
									t_blip[j].setIsDecayProp(true);
									paramAttributes * p = t_blip[j].getDecayProp();
									loadParamAttribute(XML, p);
									XML.popTag();
								}else{
									paramAttributes * p = t_blip[j].getDecaySecs();
									loadParamAttribute(XML, p);
									t_blip[j].setIsDecayProp(false);
								}
								
								if(j > 0 && j <= numAlt)XML.popTag();
								
							}
							
							XML.popTag();
						}
						
						if(XML.pushTag("POST_DECAY", 0)){
							
							int numAlt = XML.getNumTags("ALT");
							
							for(int j = 0; j < 4; j++){
								
								if(j > 0 && j <= numAlt)XML.pushTag("ALT", j-1);
								
								if(XML.pushTag("PROP", 0)){
									t_blip[j].setIsPostDecayProp(true);
									paramAttributes * p = t_blip[j].getPostDecayProp();
									loadParamAttribute(XML, p);
									XML.popTag();
								}else{
									paramAttributes * p = t_blip[j].getPostDecaySecs();
									loadParamAttribute(XML, p);
									t_blip[j].setIsPostDecayProp(false);
								}
								
								if(j > 0 && j <= numAlt)XML.popTag();
							}
							
							XML.popTag();
						}
						
						
						int numSound = XML.getNumTags("SOUND");
						int numVisual = XML.getNumTags("VISUAL");
						
						for(int j = 0; j < numSound; j ++){
							if(XML.pushTag("SOUND", j)){
								
								for(int k = 0; k < 4; k++){
									
									paramAttributes * p = t_blip[k].getSoundParam(XML.getValue("NAME", "")); 
									
									int numAlt = XML.getNumTags("ALT");
									
									if(k > 0 && k <= numAlt){
										if(XML.pushTag("ALT", k-1)){
											loadParamAttribute(XML, p);
											XML.popTag();
										}
									}else{
										loadParamAttribute(XML, p);
									}
									
								}
								XML.popTag();

							}
							
						}
						
						for(int j = 0; j < numVisual; j ++){
							if(XML.pushTag("VISUAL", j)){
								
								for(int k = 0; k < 4; k++){
									paramAttributes * p = t_blip[k].getVisualParam(XML.getValue("NAME", "")); 
									
									int numAlt = XML.getNumTags("ALT");
									
									if(k > 0 && k <= numAlt){
										if(XML.pushTag("ALT", k-1)){
											loadParamAttribute(XML, p);
											XML.popTag();
										}
									}else{
										loadParamAttribute(XML, p);
									}

									
								}
								
								XML.popTag();
							}
							
						}
						
						
						XML.popTag(); // blip pop
						
						for(int k = 0; k < 4; k++)presets[k].push_back(t_blip[k]);

					}
					
				} //loop end
			
				
				XML.popTag(); //outer tags
			}
			XML.popTag();
		}
	
	}else{
	
		cout << "file not found \n";
	}
	
	
}

void testApp::loadParamAttribute(ofxXmlSettings XML, paramAttributes * p){
	
	if(XML.tagExists("SET_TYPE", 0))p->setType = e_setType(XML.getValue("SET_TYPE", PSET_FIXED));
	if(XML.tagExists("ABS_VAL", 0))p->abs_value = XML.getValue("ABS_VAL", 0.5f);
	if(XML.tagExists("MIN_VAL", 0))p->min_val = XML.getValue("MIN_VAL", 0.0f);
	if(XML.tagExists("MAX_VAL", 0))p->max_val = XML.getValue("MAX_VAL", 1.0f);
	if(XML.tagExists("SLAVE_TO",0))p->slaveTo = XML.getValue("SLAVE_TO", "");
	
}



//--------------------------------------------------------------
void testApp::update(){
	
	//refactor
	
	world.update();
	
	cam.followReader(currentReader, world.getWorldDims());
	cam.incrementRotations();
	
	//coordinate picking
	
	cam.begin(viewPort);
	mouseP_prev.set(mouseP);
	mouseP.set(mouseX, mouseY);
	mousePick = cam.getZPlaneProjection(mouseP, viewPort);
	for(int i = 0; i < VP_coords.size(); i++){
		VPW_coords[i] = cam.getZPlaneProjection(VP_coords[i],viewPort);
	}
	cam.end();
	
	//for roi : find smallest x, largest x, smallest y, largest y
	//to create a bounding box
	
	ofVec2f world_dims(world.getWorldDims());
	
	ofVec2f tl(world_dims.x * 1.5, world_dims.y * 1.5);
	ofVec2f br(-world_dims.y * 1.5, -world_dims.y * 1.5);
	
	for(int i = 0; i < VP_coords.size(); i++){
		if(VPW_coords[i].x < tl.x)tl.x = VPW_coords[i].x;
		if(VPW_coords[i].y < tl.y)tl.y = VPW_coords[i].y;
		if(VPW_coords[i].x > br.x)br.x = VPW_coords[i].x;
		if(VPW_coords[i].y > br.y)br.y = VPW_coords[i].y;
	}
	
	roi.set(tl.x, tl.y, abs(br.x-tl.x), abs(br.y-tl.y)); 
	
	//mouseW used for all interaction
	
	mouseW.set(utils::moduloPoint(mousePick, world.getWorldDims()));
	
	if(!isMouseDown){
		
		if(mouseMode == MODE_ADD_BLIP || mouseMode == MODE_ADD_TRACK){
			world.selectSomething(ofVec2f(mouseW.x,mouseW.y));
		}
		
	}

	
}


//--------------------------------------------------------------
void testApp::draw(){
	
	ofBackground(255);
	
	cam.begin(viewPort);
	
	ofSetColor(255, 0, 0);
	
	ofVec2f world_dims = world.getWorldDims();
	for(int i = 0; i < 3; i ++){
		for(int j = 0; j < 3; j++){
			
			ofVec2f p(-world_dims.x + world_dims.x * i,
					  -world_dims.y + world_dims.y * j);
				   
			
			world.draw(p, roi, testCols[i * 3 + j]);
		}
	}
	
	cam.end();
	
	
	ofSetColor(0);
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(),2), 900,20);
	ofDrawBitmapString("mode: " + getModeString(mouseMode), 20,20);
	ofDrawBitmapString("blipPreset: " + presets[selectedPreset[1]][selectedPreset[0]].getName(), 400,20);
	ofDrawBitmapString("readerMode: " + currentReader->getModeString(), 700,20);
	ofEnableAlphaBlending();
	ofSetColor(0, 20);
	ofFill();
	ofRect(0,0,ofGetScreenWidth(),30);
	ofDisableAlphaBlending();
	
	ofSetColor(0);
	
	if(isMouseDown && currentAction == ACTION_ADD_BLIP){
		ofLine(mouseP_down,mouseP);
		ofVec2f dir(mouseP - mouseP_down);
		dir.normalize();
		vector<string> pstring = world.getPreviewParams();
		
		for(int i = 0; i <pstring.size(); i ++)ofDrawBitmapString( pstring[i], mouseP + ofVec2f(dir.x * 20, dir.y * 20 + i * 20));
	}
	
}


string testApp::getModeString(e_mouseMode temp){
	
	switch(temp){
			
		case MODE_DRAG:return "drag";break;
		case MODE_ADD_BLIP:return "add_blip";break;
		case MODE_ADD_TRACK:return "add_track";break;
		case MODE_INSERT_SPACE:return "insert_space";break;
		default:return "none";break;
			
	}
	
}



void testApp::startAction(){
	
	ofVec2f p(mouseX,mouseY);
	cam.pauseFollow();
	
	if(currentAction == ACTION_DRAG){
		
		//nothing to do here
		
	}else if(currentAction == ACTION_ADD_SHORT_TRACK ||currentAction == ACTION_ADD_LONG_TRACK){
		
		world.beginTrack(ofVec2f(mouseW.x,mouseW.y));
		
	}else if(currentAction == ACTION_ADD_BLIP){
		
		selectedPreset[1] = (isOptionKey)? buttonMode + 2: buttonMode;
		world.beginBlip(ofVec2f(mouseW.x,mouseW.y), presets[selectedPreset[1]][selectedPreset[0]]);
		
	}else if(currentAction == ACTION_INSERT_SPACE){
		
		ofVec2f orientation = (cam.getRotation(2) > 0)? ofVec2f(0,1):ofVec2f(1,0);
		world.beginInsertion(ofVec2f(mouseW.x,mouseW.y), orientation);
	
	}
	
}

void testApp::continueAction(){
	
	ofVec2f w_dir(mousePick - mousePick_down);
	
	if(currentAction == ACTION_DRAG){
		if(mouseP != mouseP_prev){
			cam.drag(mouseP_prev, mouseP, mouseW);
		}
		
	}else if(currentAction == ACTION_ADD_SHORT_TRACK){
		
		world.calcTrack(ofVec2f(mouseW.x,mouseW.y), w_dir, 0);
		
	}else if(currentAction == ACTION_ADD_LONG_TRACK){
		
		world.calcTrack(ofVec2f(mouseW.x,mouseW.y),w_dir, 1);
		
	}else if(currentAction == ACTION_ADD_BLIP){
		
		//nb s_dir is not rotated with the camera
		ofVec2f s_dir(mouseP - mouseP_down);
		float s_angle = abs(s_dir.angle(ofVec2f(0,1)));
		world.calcBlip(ofVec2f(mouseW.x,mouseW.y),w_dir, s_angle);
		if(s_dir.length() > 1){
			presets[selectedPreset[1]][selectedPreset[0]].setUserVals(w_dir.length(),s_angle); 
			//store the current user vals in the preset
		}
		
	}else if(currentAction == ACTION_INSERT_SPACE){
		
		world.resizeInsertion(w_dir.length());
														 
	}
														 
}

void testApp::endAction(){
	
	if(currentAction == ACTION_DRAG){
		
		cam.restartFollow();
		
	}else if(currentAction == ACTION_ADD_SHORT_TRACK || currentAction == ACTION_ADD_LONG_TRACK){
		
		isPreview = false;
		cam.restartFollow();
		world.endTrack();
		
	}else if(currentAction == ACTION_ADD_BLIP){
		
		isPreview = false;
		cam.restartFollow();
		world.endBlip();
		
	}else if(currentAction == ACTION_INSERT_SPACE){
		
		cam.restartFollow();	
		world.endInsertion();
		
	}
	
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
	
	for(int i = 1; i < MODE_COUNT; i ++){
		if(key == 48 + i ){
			mouseMode = e_mouseMode(i);
			world.deselectAll();	
		}
	}
	
	if(key == ' ')cam.toggleFollow();
	
	if(key == 9)isOptionKey = true;
	if(key == OF_KEY_UP)selectedPreset[0] = min(selectedPreset[0] + 1, (int)presets[0].size() - 1);
	if(key == OF_KEY_DOWN)selectedPreset[0] = max(selectedPreset[0] - 1, 0);
	
	if(key == 'r' || key == 'R')currentReader->incrementMode();
	
	if(key == 'f')ofToggleFullscreen();
	if(key == 's')world.toggleScreenData();
	if(key == 'n')world.toggleNodeData();
	if(key == 't')world.toggleTrackData();
	if(key == 'b')world.toggleBlipData();
	
	if(key == 'z'){cam.setTargetRotation((cam.getRotation(2) > 0)? 0: 90, 2);}

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
	if(isOptionKey)isOptionKey = false;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
	isMouseDown = true;
	mouseP_down.set(mouseP);
	mousePick_down.set(mousePick);
	
	switch (button) {
		case 0:buttonMode = 0;break;
		default:buttonMode = 1;break;
	}
	
	if(mouseMode == MODE_DRAG){
		currentAction = ACTION_DRAG;
	}else if(mouseMode == MODE_ADD_BLIP){
		currentAction =  ACTION_ADD_BLIP;
	}else if(mouseMode == MODE_ADD_TRACK){
		currentAction = (button == 0 )? ACTION_ADD_SHORT_TRACK: ACTION_ADD_LONG_TRACK;
	}else if(mouseMode == MODE_INSERT_SPACE){
		currentAction = ACTION_INSERT_SPACE;
	}
	
	startAction();
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	continueAction();

}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
	endAction();
	
	isMouseDown = false;
	currentAction = ACTION_NONE;
	
	mouseP_down.set(0,0);
	mouseP.set(0,0);
}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
	
}

void testApp::exit(){
	
	ofxOscMessage m;
	m.setAddress("/init");
	sender.sendMessage(m);
	
}
