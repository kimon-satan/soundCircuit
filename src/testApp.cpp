#include "testApp.h"

bool testApp::drawData =false;

//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(true);
	viewPort.set(0,0,ofGetScreenWidth(),ofGetScreenHeight());
	mouse_offset.set(0,0);
	currentLayer.setDims(ofVec2f(ofGetScreenWidth() * 1, ofGetScreenHeight() * 1));

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
	
	setupDummyPresets();
	selectedPreset = 0;
	
	thisReader.setLayer(&currentLayer);
	thisReader.setOscSender(sender);
	
	currentLayer.getSM()->beginTrack(ofVec2f(0,0));
	currentLayer.getSM()->calcTrack(ofVec2f(100,0),ofVec2f(100,0), 1);
	currentLayer.getSM()->endTrack();
	
}

void testApp::setupDummyPresets(){

	blipPreset asrHard;
	asrHard.setName("asrHard");
	asrHard.setSynthDef("basic");
	asrHard.setDrawType(BT_TESTBLIP);
	asrHard.setEnvType(ENV_ASR);
	asrHard.getAttackSecs().abs_value = 0.01;
	asrHard.getDecaySecs().abs_value = 0.05;
	asrHard.getLength().setType = PSET_USERA;
	asrHard.getSoundParam("freq").setType = PSET_USERB;
	asrHard.getVisualParam("hue").setType = PSET_USERB;

	presets.push_back(asrHard);
	
	blipPreset asrSoft;
	asrSoft.setName("asrSoft");
	asrSoft.setSynthDef("basic");
	asrSoft.setDrawType(BT_TESTBLIP);
	asrSoft.setEnvType(ENV_ASR);
	asrSoft.getAttackSecs().abs_value = 0.4;
	asrSoft.getDecaySecs().abs_value = 0.01;
	asrSoft.getLength().setType = PSET_USERA;
	asrSoft.getSoundParam("freq").setType = PSET_USERB;
	asrSoft.getVisualParam(0).setType = PSET_USERB;
	presets.push_back(asrSoft);
	
	blipPreset ar;
	ar.setName("ar");
	ar.setEnvType(ENV_AR);
	ar.setSynthDef("basic");
	ar.setDrawType(BT_TESTBLIP);
	ar.getAttackSecs().abs_value = 0.01;
	ar.getDecaySecs().abs_value = 1;
	ar.getLength().setType = PSET_FIXED;
	ar.getLength().abs_value = 20;
	ar.getSoundParam("freq").setType = PSET_MAP;
	ar.getVisualParam(0).setType = PSET_MAP;
	presets.push_back(ar);
	
	blipPreset elecClip;
	elecClip.setName("elecClip");
	elecClip.setSynthDef("clip");
	elecClip.setEnvType(ENV_ASR);
	elecClip.setDrawType(BT_ELEC_CURRENT);
	elecClip.getAttackSecs().abs_value = 0.01;
	elecClip.getDecaySecs().abs_value = 0.2;
	
	elecClip.getLength().setType = PSET_FIXED;
	elecClip.getLength().abs_value = 100;
	elecClip.getSoundParam("amp").setType = PSET_USERA;
	elecClip.getVisualParam("height").setType = PSET_USERA;
	
	elecClip.getSoundParam("freq").setType = PSET_USERB;
	elecClip.getVisualParam("density").setType = PSET_USERB;
	
	elecClip.getSoundParam("clip").setType = PSET_FIXED;
	elecClip.getSoundParam("clip").abs_value = 0.2;
	
	presets.push_back(elecClip);
	

	blipPreset strawClip;
	strawClip.setName("strawClip");
	strawClip.setSynthDef("clip");
	strawClip.setDrawType(BT_STRAW);
	strawClip.setEnvType(ENV_AR);
	
	strawClip.getLength().setType = PSET_FIXED;
	strawClip.getLength().abs_value = 10;
	
	strawClip.getAttackSecs().abs_value = 0.01;
	strawClip.getDecaySecs().abs_value = 1;
	
	strawClip.getSoundParam("freq").setType = PSET_USERB;
	strawClip.getSoundParam("amp").setType = PSET_USERA;
	strawClip.getSoundParam("clip").setType = PSET_FIXED;
	strawClip.getSoundParam("clip").abs_value = 0;
	
	strawClip.getVisualParam("rotate").setType = PSET_USERB;
	strawClip.getVisualParam("height").setType = PSET_USERA;
	
	presets.push_back(strawClip);

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
		
		if(mouseMode == MODE_ADD_TRACK){
			currentLayer.selectSomething(getWorldCoordinate(ofVec2f(mouseX, mouseY)));
		}else if(mouseMode == MODE_ADD_BLIP){
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
	ofEnableAlphaBlending();
	ofSetColor(0, 20);
	ofFill();
	ofRect(0,0,ofGetScreenWidth(),30);
	ofDisableAlphaBlending();
	
	ofSetColor(0);
	
	if(mouseDown && mouseMode == MODE_ADD_BLIP){
		ofLine(mouse_a,mouse_b);
		ofVec2f dir(mouse_b - mouse_a);
		dir.normalize();
		ofDrawBitmapString(currentLayer.getSM()->getPreviewParams(), mouse_b + dir * 20);
	}
	
}


string testApp::getModeString(e_mouseMode temp){
	
	switch(temp){
			
		case MODE_DRAG:return "drag";break;
		case MODE_ADD_TRACK:return "addTrack";break;
		case MODE_ADD_BLIP:return "addBlip";break;
		case MODE_HYBRID:return "addHybrid";break;
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

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
	
	for(int i = 1; i < MODE_COUNT; i ++){
		if(key == 48 + i )mouseMode = e_mouseMode(i);
		currentLayer.deselectAll();	
	}
	
	if(key == ' '){
		
		vpHist.clear();
		
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
	
	
	if(key == OF_KEY_UP)selectedPreset = min(selectedPreset + 1, (int)presets.size() - 1);
	if(key == OF_KEY_DOWN)selectedPreset = max(selectedPreset - 1, 0);
	
	if(key == 'f')ofToggleFullscreen();
	if(key == 's')currentLayer.toggleScreenData();
	if(key == 'n')currentLayer.getSM()->toggleNodeData();
	if(key == 't')currentLayer.getSM()->toggleTrackData();
	if(key == 'b')currentLayer.getSM()->toggleBlipData();
	
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
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
		
		prepPauseFollow();
		
		mouse_offset.x = -x - viewPort.x;
		mouse_offset.y = -y - viewPort.y;
		
	}else if(mouseMode == MODE_ADD_TRACK){
		
		prepPauseFollow();
		
		if(!isFixed)pauseFollow = true;
		currentLayer.getSM()->beginTrack(getWorldCoordinate(ofVec2f(x,y)));
		
	}else if(mouseMode == MODE_ADD_BLIP){
		
		prepPauseFollow();
		
		currentLayer.getSM()->beginBlip(getWorldCoordinate(ofVec2f(x,y)), presets[selectedPreset]);
	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	mouse_b.set(x,y);
	ofVec2f dir = mouse_b - mouse_a;

	
	if(mouseMode == MODE_DRAG){
		
		viewPort.x = -mouse_offset.x - x;
		viewPort.y = -mouse_offset.y - y;
		
		trans.x = -mouse_offset.x - x;
		trans.y = -mouse_offset.y - y;
		
	}else if(mouseMode == MODE_ADD_TRACK){
		
		currentLayer.getSM()->calcTrack(getWorldCoordinate(ofVec2f(x,y)),dir,button);
		
	}else if(mouseMode == MODE_ADD_BLIP){
	
		currentLayer.getSM()->calcBlip(getWorldCoordinate(ofVec2f(x,y)),dir);
		if(dir.length() > 1){
			presets[selectedPreset].setUserVals(dir.length(), abs(dir.angle(ofVec2f(0,1)))); //store the current user vals in the preset
		}
	
	}
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
	mouseDown = false;
	
	if(mouseMode == MODE_DRAG){
		
		if(!isFixed){
			pauseFollow = false;
			trans -= thisReader.getPos();
		}
		
	}else if(mouseMode == MODE_ADD_TRACK){
		
		isPreview = false;
		if(!isFixed){
			pauseFollow = false;
			trans -= thisReader.getPos();
		}
		currentLayer.getSM()->endTrack();
	
	}else if(mouseMode == MODE_ADD_BLIP){
		
		isPreview = false;
		if(!isFixed){
			pauseFollow = false;
			trans -= thisReader.getPos();
		}
		currentLayer.getSM()->endBlip();
		
	}
	
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
