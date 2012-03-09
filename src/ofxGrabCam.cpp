//
//  ofxGrabCam.cpp
//  ofxGrabCam
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//

#include "ofxGrabCam.h"

//--------------------------
ofxGrabCam::ofxGrabCam() : initialised(true){

	ofCamera::setNearClip(0.1);
	reset();
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


//--------------------------
void ofxGrabCam::update(ofEventArgs &args) {
	
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

ofVec3f ofxGrabCam::getZPlaneProjection(ofVec2f screenPoint, ofRectangle viewport)
{
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





