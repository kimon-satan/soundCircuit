#include "testApp.h"

bool testApp::drawData = false;


//--------------------------------------------------------------
void testApp::setup(){
	
	//basics
	
	
	sender.setup( HOST, PORT );
	ofxOscMessage m;
	m.setAddress("/init");
	sender.sendMessage(m);
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofSetCircleResolution(50);
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
	isTrack = false;
	isReader = false;
	mouseMode = MODE_ADD;
	currentAction = ACTION_NONE;
	buttonMode = 0;
	
	//preset stuff
	
	blipPreset::thisSynthDef.loadDictionary();
	
	bank tmp;
	tmp.name = "allPresets";
	banks.push_back(tmp);
	currentBank = 0;
	
	ofxXmlSettings XML;
	
	if(XML.loadFile("presets.xml")){
		
		if(XML.pushTag("SOUND_CIRCUIT", 0)){
	
			loadPresets(XML);
			loadBanks(XML);
			XML.popTag();
			
		}
		
	}else{
		
		cout << "file not found \n";
	}
	
	
	//the world
	
	world.setWorldDims(ofVec2f(1500,1500));
	world.setOSC(&sender);
	currentReader = NULL;
	oldReader = NULL;
	
	/*world.beginTrack(ofVec2f(0,0));
	world.calcTrack(ofVec2f(1,0),ofVec2f(1,0), 1);
	world.endTrack();
	currentReader = world.addReader(ofVec2f(5,0));*/
	
	
}

void testApp::loadPresets(ofxXmlSettings XML){
			
	if(XML.pushTag("BLIP_PRESETS", 0)){
		
		int numBlips = XML.getNumTags("BLIP");
		
		for(int i = 0; i < numBlips; i++){
			
			if(XML.pushTag("BLIP", i)){
				
				vector<blipPreset> t_blip;
				blipPreset tmp;
				for(int t = 0; t < 4; t++)t_blip.push_back(tmp);
				
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
				
				banks[0].presets.push_back(t_blip);
				
			}
			
		} //loop end
		
		
		XML.popTag(); //outer tags
	}
		
}


void testApp::loadBanks(ofxXmlSettings XML){

	if(XML.pushTag("BLIP_BANKS",0)){
		
		int numBnk = XML.getNumTags("BANK");
		
		for(int i = 0; i < numBnk; i ++){
			
			if(XML.pushTag("BANK", i)){
				
				bank tmpBnk;
				tmpBnk.name = XML.getValue("NAME", "noName");
				
				int numPresets = XML.getNumTags("PRESET");
				
				for(int j = 0; j < numPresets; j ++){
					
					string t_name = XML.getValue("PRESET", "", j);
					vector<blipPreset> p;
					
					for(int k = 0; k < banks[0].presets.size(); k++){
						if(t_name == banks[0].presets[k][0].getName()){
						
							p = banks[0].presets[k];
							tmpBnk.presets.push_back(p);
							break;
						}
					}
				
				}
				
				banks.push_back(tmpBnk);
				XML.popTag();
			}
		}
		
		XML.popTag();
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
	
	cam.setWorldDims(world.getWorldDims());
	cam.followReader(currentReader, viewPort);
	cam.incrementRotations();
	
	//coordinate picking
	
	cam.begin(viewPort);

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
		
		switch (mouseMode) {
			case MODE_ADD:
				if(isReader){
					currentSelection = world.selectSomething(ofVec2f(mouseW.x,mouseW.y), true, false, false, false);
				}else{
					currentSelection = world.selectSomething(ofVec2f(mouseW.x,mouseW.y), false, false, true, true);
				}
				break;
			case MODE_ADJUST:
				if(isReader){
					currentSelection = world.selectSomething(ofVec2f(mouseW.x,mouseW.y), true, false, false, false);
				}else{
					currentSelection = world.selectSomething(ofVec2f(mouseW.x,mouseW.y), false, true, true, true);
				}
				break;
			case MODE_DESTROY:
				if(isReader){
					currentSelection = world.selectSomething(ofVec2f(mouseW.x,mouseW.y), true, false, false, false);
				}else{
					currentSelection = world.selectSomething(ofVec2f(mouseW.x,mouseW.y), false, true, false, false);
				}
				break;
		
		}
		
		
		
	}else{
	
		mouseDownCount += 1;
	}

	
}


//--------------------------------------------------------------
void testApp::draw(){
	
	ofBackground(255);
	

	
	cam.begin(viewPort);
	
	ofSetColor(255, 0, 0);
	
	ofVec2f world_dims = world.getWorldDims();
    
    for(int m =0; m < 6; m++){
        for(int i = 0; i < 3; i ++){
            for(int j = 0; j < 3; j++){
                
                ofVec2f p(-world_dims.x + world_dims.x * i,
                          -world_dims.y + world_dims.y * j);
                       
                world.pushRender(p, roi);
                
                switch(m){
                    case 0: world.drawDebug(testCols[i * 3 + j], mousePick);
                    case 1: world.drawTracks();break;
                    case 2: world.drawBlips();break;
                    case 3: world.drawNodes();break;
                    case 4: world.drawSelected();break; //(cf with mousePick)
                    case 5: world.drawReaders();break;
                        
                }
                
                world.popRender();
                
            }
        }
    }
  
	
	cam.end();
		
	
	ofSetColor(0);
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(),2), 900,20);
	ofDrawBitmapString("mode: " + getModeString(mouseMode), 20,20);
	ofDrawBitmapString("blipPreset: " + banks[currentBank].getSelectedName(), 450,20);
	ofDrawBitmapString("bank: " + banks[currentBank].name, 200,20);
	

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
			
		case MODE_ADD:return "add";break;
		case MODE_ADJUST:return "adjust";break;
		case MODE_DESTROY: return "destroy"; break;
		default:return "none";break;
			
	}
	
}



void testApp::startAction(){
	
	ofVec2f p(mouseX,mouseY);
		
	
	if(currentAction == ACTION_DRAG){
		
		//nothing to do here
		
	}else if(currentAction == ACTION_ADD_SHORT_TRACK ||currentAction == ACTION_ADD_LONG_TRACK){
		
		world.beginTrack(ofVec2f(mouseW.x,mouseW.y));
		
	}else if(currentAction == ACTION_ADD_BLIP){
		
		banks[currentBank].selectedPreset[1] = (isOptionKey)? buttonMode + 2: buttonMode;
		world.beginBlip(ofVec2f(mouseW.x,mouseW.y), banks[currentBank].getSelectedPreset());
		
	}else if(currentAction == ACTION_INSERT_SPACE){
		
		ofVec2f orientation = (cam.getRotation(2) > 0)? ofVec2f(0,1):ofVec2f(1,0);
		world.beginInsertSpace(ofVec2f(mouseW.x,mouseW.y), orientation);
	
	}else if(currentAction == ACTION_STRETCH_SPACE){
		
		ofVec2f orientation = (cam.getRotation(2) > 0)? ofVec2f(0,1):ofVec2f(1,0);
		world.beginInsertion(ofVec2f(mouseW.x,mouseW.y), orientation);
		
	}else if(currentAction == ACTION_ADJUST_NODE){
		
		world.beginNode(ofVec2f(mouseW.x,mouseW.y), buttonMode);
		
	}else if(currentAction == ACTION_ADD_READER){
		
		int oldIndex = (currentReader)? currentReader->getIndex() : -99;
		currentReader = world.addReader(ofVec2f(mouseW.x, mouseW.y));
		if(oldIndex != -99)oldReader = world.getReaderRef(oldIndex);
		cam.calcFollowPoint(currentReader, viewPort);
		cam.calcTrans();
		
	}else if(currentAction == ACTION_FOLLOW_READER){

		currentReader = world.getNearestReader(ofVec2f(mouseW.x,mouseW.y));

		
	}else if(currentAction == ACTION_DESTROY_READER){
		
		int currentIndex = (currentReader) ? currentReader->getIndex() : -99;
		reader * t = world.getNearestReader(ofVec2f(mouseW.x,mouseW.y));
		if(!t)return;
		if(t->getIndex() == currentIndex)currentIndex = -99;
		world.destroyReader(t);
		if(currentIndex == -99){ 
			currentReader = world.getNearestReader(ofVec2f(mouseW.x,mouseW.y));
		}else{ 
			currentReader = world.getReaderRef(currentIndex);
		}
		
	}else if(currentAction == ACTION_ADJUST_READER){
		
		currentReader = world.getNearestReader(ofVec2f(mouseW.x,mouseW.y));
		if(currentReader)currentReader->beginAdjust();
		
	}
	
	if(!isReader){
		cam.pauseFollow();
	}else{
		
		if(cam.getIsFixed())cam.toggleFollow();
		cam.calcFollowPoint(currentReader, viewPort);
		cam.calcTrans();
		
	}
	

	
}

void testApp::continueAction(){
	
	ofVec2f w_dir(mousePick - mousePick_down);
	
	if(currentAction == ACTION_DRAG){
		//for some reason doesn't work when loads of blips ?
		//solved by updating mouseP in mouseDragged method
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
			banks[currentBank].getSelectedRef().setUserVals(min((float)M_VAL * WORLD_UNIT, w_dir.length()),s_angle); 
			//store the current user vals in the preset
		}
		
	}else if(currentAction == ACTION_INSERT_SPACE || currentAction == ACTION_STRETCH_SPACE){
		
		world.resizeInsertion(w_dir.length());
														 
	}else if(currentAction == ACTION_ADJUST_NODE){
	
		world.calcNode(ofVec2f(mouseW.x,mouseW.y));
		
	}else if(currentAction == ACTION_ADJUST_READER){
	
		if(currentReader)currentReader->adjust(ofVec2f(mouseW.x, mouseW.y));
		
	}else if(currentAction == ACTION_ADD_READER){
		if(currentReader){
			if(!currentReader->getIsAdjusting())currentReader->beginAdjust();
			currentReader->adjust(ofVec2f(mouseW.x, mouseW.y));
		}
	}

														 
}

void testApp::endAction(){
	
	
	
	if(currentAction == ACTION_DRAG){
		
		
	}else if(currentAction == ACTION_ADD_SHORT_TRACK || currentAction == ACTION_ADD_LONG_TRACK){
		
		isPreview = false;
		world.endTrack();
		
	}else if(currentAction == ACTION_ADD_BLIP){
		
		isPreview = false;
		world.endBlip();
		
	}else if(currentAction == ACTION_INSERT_SPACE || currentAction == ACTION_STRETCH_SPACE){
		
		world.endInsertion();
		
	}else if(currentAction == ACTION_ADJUST_NODE){
	
		world.endNode();
		
	}else if(currentAction == ACTION_FOLLOW_READER){
		
		world.deselectReaders();
		
	}else if(currentAction == ACTION_ADJUST_READER){
		
		if(currentReader){
			currentReader->endAdjust();
		}
		
	}else if(currentAction == ACTION_ADD_READER){
		
		if(currentReader){
			if(currentReader->getIsAdjusting())currentReader->endAdjust();
		}else if(oldReader){
			currentReader = oldReader;
			cam.calcFollowPoint(currentReader, viewPort);
			cam.calcTrans();
		}
		
		oldReader = NULL;
		
		
	}
	
	if(!isReader)cam.restartFollow();

}
//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
	
	for(int i = 0; i < MODE_COUNT; i ++){
		if(key == 49 + i ){
			mouseMode = e_mouseMode(i);
			world.deselectAll();	
		}
	}
	
	if(key == ' '){
		if(!isMouseDown)cam.toggleFollow();
	}
	
	if(key == 9)isOptionKey = true;
	
	if(key == OF_KEY_UP){
		banks[currentBank].incrementPreset();
		mouseMode = MODE_ADD;
	}
	if(key == OF_KEY_DOWN){
		banks[currentBank].decrementPreset();
		mouseMode = MODE_ADD;
	}
	
	if(key == OF_KEY_LEFT)currentBank = max(0, currentBank - 1);
	if(key == OF_KEY_RIGHT)currentBank = min(currentBank + 1, (int)banks.size() - 1);
	
	if(key == 't')isTrack = true;
	if(key == 'r')isReader = true;

	
	//debug keys
	if(key == 'f')ofToggleFullscreen();

	if(key == 'D'){
		world.toggleNodeData();
		world.toggleTrackData();
		world.toggleBlipData();
		world.toggleScreenData();
		drawData = !drawData;
	}
	
	if(key == 'z'){cam.setTargetRotation((cam.getRotation(2) > 0)? 0: 90, 2);}
	if(key == 'x'){cam.setTargetRotation((cam.getRotation(0) > 0)? 0 : 25, 0);}
	if(key == 'v'){cam.zoom(-5);}
	if(key == 'c'){cam.zoom(5);}
	//if(key == 'y'){cam.setTargetRotation((cam.getRotation(1) > 0)? 0 : 25, 1);}
	

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
	if(isOptionKey)isOptionKey = false;
	isTrack = false;
	isReader = false;

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	mouseP_prev.set(mouseP);
	mouseP.set(x, y);
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
	isMouseDown = true;
	mouseP_down.set(mouseP);
	mousePick_down.set(mousePick);
	mouseDownCount = 0;
	
	switch (button) {
		case 0:buttonMode = 0;break;
		default:buttonMode = 1;break;
	}
	
	
	
	switch (mouseMode) {
		case MODE_ADD:
			if(isReader){
				currentAction = (buttonMode == 0)? ACTION_ADD_READER: ACTION_FOLLOW_READER;
			}else if(isTrack){
				currentAction = (button == 0 )? ACTION_ADD_SHORT_TRACK: ACTION_ADD_LONG_TRACK;
			}else{
				if(currentSelection == OT_TRACK){
					currentAction = ACTION_ADD_BLIP;
				}else if(currentSelection == OT_NODE){
					currentAction = ACTION_ADJUST_NODE;
				}else if(currentSelection == OT_WORLD){
					currentAction = (buttonMode == 0) ? ACTION_DRAG : ACTION_STRETCH_SPACE;
				}
			}
			break;
			
		case MODE_ADJUST:
			
			if(isReader){
				currentAction = (buttonMode == 0)? ACTION_ADJUST_READER : ACTION_FOLLOW_READER;
			}else{
				if(currentSelection == OT_NODE){
					currentAction = ACTION_ADJUST_NODE;
				}else if(currentSelection == OT_BLIP){
					currentAction = ACTION_ADJUST_BLIP;
				}else if(currentSelection == OT_WORLD){
					currentAction = (buttonMode == 0) ? ACTION_DRAG : ACTION_STRETCH_SPACE;
				}
			}
			
			break;
		case MODE_DESTROY:
			if(isReader){
				currentAction = (buttonMode == 0)? ACTION_DESTROY_READER : ACTION_FOLLOW_READER;
			}else{
				if(currentSelection == OT_BLIP){
					currentAction = ACTION_DESTROY_BLIP;
				}else if(currentSelection == OT_WORLD){
					currentAction = (buttonMode == 0) ? ACTION_DRAG : ACTION_STRETCH_SPACE;
				}
			}
			
			break;

	}

	
	startAction();
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	mouseP_prev.set(mouseP);
	mouseP.set(x, y);
	
	if(mouseDownCount > 20 || mouseP.distance(mouseP_down) > 20)continueAction();

}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
	endAction();
	
	isMouseDown = false;
	currentAction = ACTION_NONE;
	mouseP_down.set(0,0);
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
