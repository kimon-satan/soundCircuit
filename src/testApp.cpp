#include "testApp.h"

bool testApp::drawData =false;

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetVerticalSync(true);
	viewPort.set(0,0,ofGetScreenWidth(),ofGetScreenHeight());
	mouse_offset.set(0,0);
	currentLayer.setDims(ofVec2f(ofGetScreenWidth() * 1, ofGetScreenWidth() * 1));
	
	sender.setup( HOST, PORT );
	
	ofxOscMessage m;
	m.setAddress("/init");
	sender.sendMessage(m);
	
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	isFixed = false;
	pauseFollow = false;
	isPreview = false;
	trans.set(0,0);
	
	mouseDown = false;
	isOptionKey = false;
	mouseMode = MODE_NONE;
	currentAction = ACTION_NONE;
	
	blipPreset::thisSynthDef.loadDictionary();

	loadPresets();
	selectedPreset = 0;
	
	thisReader.setLayer(&currentLayer);
	thisReader.setOscSender(&sender);
	
	currentLayer.getSM()->beginTrack(ofVec2f(0,0));
	currentLayer.getSM()->calcTrack(ofVec2f(100,0),ofVec2f(100,0), 1);
	currentLayer.getSM()->endTrack();
	
	
	
}

void testApp::loadPresets(){
	
	ofxXmlSettings XML;
	
	if(XML.loadFile("presets.xml")){
		
		if(XML.pushTag("SOUND_CIRCUIT", 0)){
			
			if(XML.pushTag("BLIP_PRESETS", 0)){
				
				int numBlips = XML.getNumTags("BLIP");
				
				for(int i = 0; i < numBlips; i++){
					
					if(XML.pushTag("BLIP", i)){
						
						blipPreset t_blip;
						t_blip.setName(XML.getValue("NAME", ""));
						t_blip.setSynthDef(XML.getValue("SYNTH", ""));
						t_blip.setDrawType(XML.getValue("DRAW", ""));
						t_blip.setEnvType(e_envType(XML.getValue("ENV", ENV_ASR)));
						
						if(XML.pushTag("LENGTH", 0)){
							paramAttributes * p = t_blip.getLength();
							loadParamAttribute(XML, p);
							XML.popTag();
						}
						
						if(XML.pushTag("ATTACK", 0)){
							
							
							if(XML.pushTag("PROP", 0)){
								t_blip.setIsAttackProp(true);
								paramAttributes * p = t_blip.getAttackProp();
								loadParamAttribute(XML, p);
								XML.popTag();
							}else{
								paramAttributes * p = t_blip.getAttackSecs();
								loadParamAttribute(XML, p);
								t_blip.setIsAttackProp(false);
							}
							
							XML.popTag();
						}
						
						if(XML.pushTag("DECAY", 0)){
					
							
							if(XML.pushTag("PROP", 0)){
								t_blip.setIsDecayProp(true);
								paramAttributes * p = t_blip.getDecayProp();
								loadParamAttribute(XML, p);
								XML.popTag();
							}else{
								paramAttributes * p = t_blip.getDecaySecs();
								loadParamAttribute(XML, p);
								t_blip.setIsDecayProp(false);
							}
							XML.popTag();
						}
						
						if(XML.pushTag("POST_DECAY", 0)){
							
							if(XML.pushTag("PROP", 0)){
								t_blip.setIsPostDecayProp(true);
								paramAttributes * p = t_blip.getPostDecayProp();
								loadParamAttribute(XML, p);
								XML.popTag();
							}else{
								paramAttributes * p = t_blip.getPostDecaySecs();
								loadParamAttribute(XML, p);
								t_blip.setIsPostDecayProp(false);
							}
							
							XML.popTag();
						}
						
						
						int numSound = XML.getNumTags("SOUND");
						int numVisual = XML.getNumTags("VISUAL");
						
						for(int j = 0; j < numSound; j ++){
							if(XML.pushTag("SOUND", j)){
								paramAttributes * p = t_blip.getSoundParam(XML.getValue("NAME", "")); 
								loadParamAttribute(XML, p);
								XML.popTag();
							}
							
						}
						
						for(int j = 0; j < numVisual; j ++){
							if(XML.pushTag("VISUAL", j)){
								paramAttributes * p = t_blip.getVisualParam(XML.getValue("NAME", "")); 
								loadParamAttribute(XML, p);
								XML.popTag();
							}
							
						}
						
						
						XML.popTag(); // blip pop
						
						presets.push_back(t_blip);
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
	if(XML.tagExists("SLAVE_TO",0))p->slaveTo = XML.getValue("SLAVE", "");
	
}


//--------------------------------------------------------------
void testApp::update(){
	
	thisReader.update();
	currentLayer.update();
	
	if(!isFixed && !pauseFollow){
		
		viewPort.x = thisReader.getPos().x + trans.x;
		viewPort.y = thisReader.getPos().y + trans.y;
		
		
		if(abs(trans.x) > 0.5){ //needs some solution for wrapping //but leave for now
			trans.x *= 0.98;
		}else{
			trans.x = 0;
		}
		
		if(abs(trans.y) > 0.5){
			trans.y *= 0.98;
		}else{
			trans.y = 0;
		}
		
		
		//position averaging ... needs more work due to wrapping
		/*ofVec2f t;  
		 t.x = thisReader.getPos().x + trans.x;
		 t.y = thisReader.getPos().y + trans.y;
		 vpHist.push_back(t);
		 if(vpHist.size() > 10){vpHist.erase(vpHist.begin());}
		 ofVec2f avPos(0,0);
		 for(int i = 0; i < vpHist.size(); i ++){
		 avPos += vpHist[i];
		 }
		 avPos /= vpHist.size();
		 
		 viewPort.x = avPos.x;
		 viewPort.y = avPos.y;*/
	}
	
	moduloViewPort();
	
	if(!mouseDown){
		
		if(mouseMode == MODE_ADD){
			currentLayer.selectSomething(getWorldCoordinate(ofVec2f(mouseX, mouseY)));
		}
		
	}
	
	updateDummyViews();
	
}

void testApp::updateDummyViews(){
	
	ofVec2f t_dims = currentLayer.getDims();
	
	float y_over = viewPort.y + viewPort.height/2.0f - t_dims.y/2.0f;
	float y_under = -viewPort.y + viewPort.height/2.0f - t_dims.y/2.0f;
	float x_over = viewPort.x + viewPort.width/2.0f - t_dims.x/2.0f;
	float x_under = -viewPort.x + viewPort.width/2.0f - t_dims.x/2.0f;
	
	dummy_views.clear();
	ofRectangle dummy_view(viewPort.x,viewPort.y,viewPort.width,viewPort.height);
	
	if( y_over > 0 ){ // viewport goes over top edge
		dummy_view.y = -t_dims.y/2 + y_over - viewPort.height/2;
		dummy_views.push_back(dummy_view);
	}else if( y_under > 0 ){ // viewport goes over top edge
		dummy_view.y = t_dims.y/2 - y_under + viewPort.height/2;
		dummy_views.push_back(dummy_view);
	}
	
	dummy_view.set(viewPort.x,viewPort.y,viewPort.width,viewPort.height);
	
	if( x_over > 0 ){ 
		dummy_view.x = -t_dims.x/2 + x_over - viewPort.width/2;
		dummy_views.push_back(dummy_view);
	}else if( x_under > 0 ){ 
		dummy_view.x = t_dims.x/2 - x_under + viewPort.width/2;
		dummy_views.push_back(dummy_view);
	}
	
	if( x_over > 0 && y_over > 0){
		dummy_view.y = -t_dims.y/2 + y_over - viewPort.height/2;
		dummy_view.x = -t_dims.x/2 + x_over - viewPort.width/2;
		dummy_views.push_back(dummy_view);
	}else if( x_over > 0 && y_under > 0){
		dummy_view.x = -t_dims.x/2 + x_over - viewPort.width/2;
		dummy_view.y = t_dims.y/2 - y_under + viewPort.height/2;
		dummy_views.push_back(dummy_view);
	}else if( x_under > 0 && y_under > 0){
		dummy_view.y = t_dims.y/2 - y_under + viewPort.height/2;
		dummy_view.x = t_dims.x/2 - x_under + viewPort.width/2;
		dummy_views.push_back(dummy_view);
	}else if( x_under > 0 && y_over > 0){
		dummy_view.x = t_dims.x/2 - x_under + viewPort.width/2;
		dummy_view.y = -t_dims.y/2 + y_over - viewPort.height/2;
		dummy_views.push_back(dummy_view);
	}
	
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofBackground(255);
	
	currentLayer.draw(viewPort);
	for(int i = 0; i < dummy_views.size(); i ++){currentLayer.draw(dummy_views[i], true);}
	
	thisReader.draw(viewPort);
	for(int i = 0; i < dummy_views.size(); i ++){thisReader.draw(dummy_views[i]);}
	
	ofSetColor(0);
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(),2), 1000,20);
	ofDrawBitmapString("mode: " + getModeString(mouseMode), 20,20);
	ofDrawBitmapString("blipPreset: " + presets[selectedPreset].getName(), 400,20);
	ofDrawBitmapString("readerMode: " + thisReader.getModeString(), 700,20);
	ofEnableAlphaBlending();
	ofSetColor(0, 20);
	ofFill();
	ofRect(0,0,ofGetScreenWidth(),30);
	ofDisableAlphaBlending();
	
	ofSetColor(0);
	
	if(mouseDown && currentAction == ACTION_ADD_BLIP){
		ofLine(mouse_a,mouse_b);
		ofVec2f dir(mouse_b - mouse_a);
		dir.normalize();
		ofDrawBitmapString(currentLayer.getSM()->getPreviewParams(), mouse_b + dir * 20);
	}
	
}


string testApp::getModeString(e_mouseMode temp){
	
	switch(temp){
			
		case MODE_DRAG:return "drag";break;
		case MODE_ADD:return "add";break;
		default:return "none";break;
			
	}
	
}

void testApp::moduloViewPort(){
	
	ofVec2f t_dims = currentLayer.getDims();
	
	if(viewPort.y < -t_dims.y/2 -viewPort.height/2 ){
		mouse_offset.y += viewPort.y;	
		viewPort.y += t_dims.y; 
		mouse_offset.y -= viewPort.y;
		//	vpHist.clear();
	}else if(viewPort.y > t_dims.y/2 + viewPort.height/2 ){
		mouse_offset.y += viewPort.y;	
		viewPort.y -= t_dims.y; 
		mouse_offset.y -= viewPort.y;
		//	vpHist.clear();
	}
	
	if(viewPort.x < -t_dims.x/2 -viewPort.width/2 ){
		mouse_offset.x += viewPort.x;	
		viewPort.x += t_dims.x; 
		mouse_offset.x -= viewPort.x;
		//vpHist.clear();
	}else if(viewPort.x > t_dims.x/2 + viewPort.width/2 ){
		mouse_offset.x += viewPort.x;	
		viewPort.x -= t_dims.x; 
		mouse_offset.x -= viewPort.x;
		//vpHist.clear();
	}
	
	
}



ofVec2f testApp::getWorldCoordinate(ofVec2f t_point){
	
	t_point.x -= ofGetScreenWidth()/2;
	t_point.y -= ofGetScreenHeight()/2;
	
	ofVec2f point(t_point);
	
	point.x += viewPort.x;
	point.y += viewPort.y;
	
	ofVec2f t_dims = currentLayer.getDims();
	
	ofRectangle scr(-t_dims.x/2,-t_dims.y/2,t_dims.x,t_dims.y);
	
	if(!scr.inside(point)){
		
		
		for(int i = 0; i < dummy_views.size(); i++){
			
			
			ofRectangle d;
			d.setFromCenter(-dummy_views[i].x, -dummy_views[i].y, t_dims.x, t_dims.y);
			
			if(d.inside(t_point)){
				
				point = t_point + ofVec2f(dummy_views[i].x, dummy_views[i].y);
			}
			
		}
		
		
	}
	
	return point;
	
}

void testApp::prepPauseFollow(){
	
	if(!isFixed){
		pauseFollow = true;
		trans += thisReader.getPos();
		moduloViewPort();
		viewPort.x = trans.x;
		viewPort.y = trans.y;
	}
	
}

void testApp::startAction(){
	
	if(currentAction == ACTION_DRAG){
		
		prepPauseFollow();
		
		mouse_offset.x = -mouseX - viewPort.x;
		mouse_offset.y = -mouseY - viewPort.y;
		
	}else if(currentAction == ACTION_ADD_SHORT_TRACK ||currentAction == ACTION_ADD_LONG_TRACK){
		
		prepPauseFollow();
		
		if(!isFixed)pauseFollow = true;
		currentLayer.getSM()->beginTrack(getWorldCoordinate(ofVec2f(mouseX,mouseY)));
		
	}else if(currentAction == ACTION_ADD_BLIP){
		
		prepPauseFollow();
		currentLayer.getSM()->beginBlip(getWorldCoordinate(ofVec2f(mouseX,mouseY)), presets[selectedPreset]);
		
	}
	
}

void testApp::continueAction(ofVec2f t_dir){
	
	if(currentAction == ACTION_DRAG){
		
		viewPort.x = -mouse_offset.x - mouseX;
		viewPort.y = -mouse_offset.y - mouseY;
		
		trans.x = -mouse_offset.x - mouseX;
		trans.y = -mouse_offset.y - mouseY;
		
	}else if(currentAction == ACTION_ADD_SHORT_TRACK){
		
		currentLayer.getSM()->calcTrack(getWorldCoordinate(ofVec2f(mouseX,mouseY)),t_dir,0);
		
	}else if(currentAction == ACTION_ADD_LONG_TRACK){
		
		currentLayer.getSM()->calcTrack(getWorldCoordinate(ofVec2f(mouseX,mouseY)),t_dir,1);
		
	}else if(currentAction == ACTION_ADD_BLIP){
		
		currentLayer.getSM()->calcBlip(getWorldCoordinate(ofVec2f(mouseX,mouseY)),t_dir);
		if(t_dir.length() > 1){
			presets[selectedPreset].setUserVals(t_dir.length(), abs(t_dir.angle(ofVec2f(0,1)))); //store the current user vals in the preset
		}
		
	}
}

void testApp::endAction(){
	
	if(currentAction == ACTION_DRAG){
		
		if(!isFixed){
			pauseFollow = false;
			trans -= thisReader.getPos();
		}
		
	}else if(currentAction == ACTION_ADD_SHORT_TRACK || currentAction == ACTION_ADD_LONG_TRACK){
		
		isPreview = false;
		if(!isFixed){
			pauseFollow = false;
			trans -= thisReader.getPos();
		}
		currentLayer.getSM()->endTrack();
		
	}else if(currentAction == ACTION_ADD_BLIP){
		
		isPreview = false;
		if(!isFixed){
			pauseFollow = false;
			trans -= thisReader.getPos();
		}
		
		currentLayer.getSM()->endBlip();
		
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
	
	for(int i = 1; i < MODE_COUNT; i ++){
		if(key == 48 + i )mouseMode = e_mouseMode(i);
		currentLayer.deselectAll();	
	}
	
	if(key == ' '){
		
		//vpHist.clear();
		
		if(isFixed){
			trans -= thisReader.getPos();
		}else{
			trans += thisReader.getPos();
			moduloViewPort();
			viewPort.x = trans.x;
			viewPort.y = trans.y;
		}
		
		isFixed = !isFixed;
	}
	
	
	if(key == 9)isOptionKey = true;
	if(key == OF_KEY_UP)selectedPreset = min(selectedPreset + 1, (int)presets.size() - 1);
	if(key == OF_KEY_DOWN)selectedPreset = max(selectedPreset - 1, 0);
	if(key == 'r')thisReader.incrementMode();
	
	if(key == 'f')ofToggleFullscreen();
	if(key == 's')currentLayer.toggleScreenData();
	if(key == 'n')currentLayer.getSM()->toggleNodeData();
	if(key == 't')currentLayer.getSM()->toggleTrackData();
	if(key == 'b')currentLayer.getSM()->toggleBlipData();
	
	
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
	
	mouseDown = true;
	mouse_a.set(x,y);
	mouse_b.set(mouse_a);
	
	if(mouseMode == MODE_DRAG){
		currentAction = ACTION_DRAG;
	}else if(mouseMode == MODE_ADD){
		currentAction = (button == 0)? ACTION_ADD_BLIP : ((isOptionKey)? ACTION_ADD_LONG_TRACK : ACTION_ADD_SHORT_TRACK);
	}
	
	startAction();
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	mouse_b.set(x,y);
	ofVec2f dir = mouse_b - mouse_a;
	continueAction(dir);
	
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
	endAction();
	
	mouseDown = false;
	currentAction = ACTION_NONE;
	
	mouse_a.set(0,0);
	mouse_b.set(0,0);
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
