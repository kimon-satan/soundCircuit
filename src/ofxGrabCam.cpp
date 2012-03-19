
#include "ofxGrabCam.h"
#include "reader.h"

//--------------------------
ofxGrabCam::ofxGrabCam() : initialised(true),kLagFrames(120),kMinIncr(1){

	reset();
	//set up camera
	
	setPosition(0, 0, 800);
	setFarClip(2000);
	setNearClip(200);
	
	ofVec3f c(0,0,0);
	orbit(180,0, getPosition().z, c);
	roll(180);
	cacheMatrices(true);
	
	isFixed = false;
	isPaused = false;
	
	trans.set(0,0);
	direction.set(1,0);
	lagCount = 0;
	
	for(int i = 0; i < 3; i++){
		targetRots[i] = 0;
		rots[i] = 0;
	}
}

//--------------------------
ofxGrabCam::~ofxGrabCam() {

}

//--------------------------
void ofxGrabCam::begin(ofRectangle viewport) {
	
	glEnable(GL_DEPTH_TEST);	
	viewportRect = viewport;
	ofCamera::begin(viewport);
	ofPushMatrix();

	glGetDoublev(GL_PROJECTION_MATRIX, this->matP);
	glGetDoublev(GL_MODELVIEW_MATRIX, this->matM);
	glGetIntegerv(GL_VIEWPORT, this->viewport);
	
}

//--------------------------
void ofxGrabCam::end() {
	
	ofPopMatrix();
	ofCamera::end();
	glDisable(GL_DEPTH_TEST);
	
}

//--------------------------
void ofxGrabCam::reset() {
	ofCamera::resetTransform();
}


void ofxGrabCam::followReader(reader * currentReader, ofRectangle t_viewPort){

	//handle camera following reader
	
	ofVec2f t_pos(getPosition().x , getPosition().y);
	
	if(currentReader){
		
		//diff is the adjustment for when the Zplane is rotated
		ofVec2f m_pos(utils::moduloPoint(t_pos, worldDims));
		ofVec2f diff(getZPlaneProjection(ofVec2f(t_viewPort.width/2,t_viewPort.height/2),t_viewPort));		
		diff -= m_pos;
		followPoint.set(currentReader->getPos() - diff);
		
	}
	
	if(!isFixed && !isPaused && currentReader){
		
		//needs t_pos
		if(currentReader->getIsNewDirection())lagCount = kLagFrames;
		
		t_pos.x = followPoint.x + trans.x;
		t_pos.y = followPoint.y + trans.y;
		
		if(lagCount > 1){
			trans -= currentReader->getIncrement() * (float)(lagCount/kLagFrames)  * currentReader->getDirection();
			trans += currentReader->getIncrement()  * (float)(lagCount/kLagFrames) * direction;
			lagCount -= 1;
		}else if(lagCount ==  1){
			direction = currentReader->getDirection();
			lagCount = 0;
		}
		
		if(abs(trans.x) > kMinIncr * 2){ 
			float incr = (trans.x > 0)? max(kMinIncr,trans.x * 0.02f):min(-kMinIncr,trans.x * 0.02f);
			trans.x -= incr;
		}else{
			trans.x = 0;
		}
		
		if(abs(trans.y) > kMinIncr * 2){ 
			float incr = (trans.y > 0)? max(kMinIncr,trans.y * 0.02f):min(-kMinIncr,trans.y * 0.02f);
			trans.y -= incr;
		}else{
			trans.y = 0;
		}
		
	}
	
	t_pos = utils::moduloPoint(t_pos, worldDims);
	setPosition(t_pos.x, t_pos.y, getPosition().z);
	
}

//--------------------------
void ofxGrabCam::incrementRotations() {
	
	//handle rotations
	
	for(int i = 0; i < 3; i ++){
		if(abs(rots[i] - targetRots[i]) > 0){
			
			if(rots[i] < targetRots[i]){ 
				
				switch(i){
					case 0: tilt(1);break;
					case 1: pan(1); break;
					case 2: rotate(-1,0,0,1);break;
				}
				rots[i] += 1;
				
			}else{
				
				switch(i){
					case 0: tilt(-1);break;
					case 1: pan(-1); break;
					case 2: rotate(1,0,0,1);break;
				}
				rots[i] -= 1;
			}
			
		}else{
			
			rots[i] = targetRots[i];
		}
	}
	
}


void ofxGrabCam::pauseFollow(){

	if(!isFixed)isPaused = true;
}

void ofxGrabCam::restartFollow(){
	
	if(!isFixed){  
		isPaused = false;
		trans = ofVec2f(getPosition().x, getPosition().y) - followPoint;
		trans = utils::moduloPoint(trans, worldDims);
	}

}

void ofxGrabCam::toggleFollow(){
	
	trans = ofVec2f(getPosition().x, getPosition().y) - followPoint;
	trans = utils::moduloPoint(trans, worldDims);
	isFixed = !isFixed;
}

void ofxGrabCam::drag(ofVec2f p_origin, ofVec2f p_now, ofVec3f mouseW){
	
	ofVec2f t_dist(p_now - p_origin);
	
	float dx = t_dist.x / ofGetViewportWidth();
	float dy = t_dist.y / ofGetViewportHeight();
	
	ofVec3f p = ofCamera::getPosition();
	ofVec3f uy = ofCamera::getUpDir();
	ofVec3f ux = ofCamera::getSideDir();
	float ar = float(ofGetViewportWidth()) / float(ofGetViewportHeight());
	
	//pan
	float d = (p - mouseW).length();
	
	ofCamera::move(dx * -ux * d * ar);
	ofCamera::move(dy * uy * d);
} 

//--------------------------
ofVec3f ofxGrabCam::pickCoordinate(ofVec2f t_mouseP) {
	//read z value from depth buffer at mouse coords
	
	ofVec3f t_mouseW(t_mouseP.x, t_mouseP.y, 0);
	
	glReadPixels(t_mouseW.x, ofGetScreenHeight()-1-t_mouseP.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &t_mouseW.z);
	
	if (t_mouseW.z == 1.0f){
		return ofVec3f(-99,-99,-99); //represents point not found
	}
	
	GLdouble c[3];
	
	gluUnProject(t_mouseW.x, ofGetScreenHeight()-1-t_mouseW.y, t_mouseW.z, matM, matP, viewport, c, c+1, c+2);
	
	ofVec3f mouseW(c[0],c[1],c[2]);
	
	return mouseW;
}

ofVec3f ofxGrabCam::unPickCoordinate(ofVec3f t_mouseW){
	
	GLdouble c[3];
	
	gluProject(t_mouseW.x, t_mouseW.y, t_mouseW.z, 
				matM, matP, viewport, 
			   c, c+1, c+2);
	
	ofVec3f mouseP(c[0],c[1],c[2]);
	
	return mouseP;


}



ofVec3f ofxGrabCam::getZPlaneProjection(ofVec2f screenPoint, ofRectangle viewport){
	
	ofVec3f ray[2];
	
	//define ray in screen space
	ray[0] = ofVec3f(screenPoint.x, screenPoint.y, -1);
	ray[1] = ofVec3f(screenPoint.x, screenPoint.y, 1);
	
	//transform ray into world space
	ray[0] = screenToWorld(ray[0], viewport);
	ray[1] = screenToWorld(ray[1], viewport);
	
	ofVec3f rayvec(ray[1]-ray[0]);
	ofVec3f intersect;
	
	float u = -1 * ray[0].z/rayvec.z; //for some reason it needs to be reversed
	intersect.x = ray[0].x + rayvec.x * u;
	intersect.y = ray[0].y + rayvec.y * u;
	intersect.z = 0;
	
	return intersect;
}


void ofxGrabCam::setTargetRotation(float t_rot, int t_axis){

	targetRots[t_axis] = t_rot;
}

float ofxGrabCam::getRotation(int t_axis){

	return rots[t_axis];

}


bool ofxGrabCam::getIsFixed(){return isFixed;}

ofVec2f ofxGrabCam::getTrans(){return trans;}
void ofxGrabCam::setWorldDims(ofVec2f t_dims){worldDims.set(t_dims);}
