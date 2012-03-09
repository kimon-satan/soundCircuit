#pragma once
//
//  ofxGrabCam.h
//  ofxGrabCam
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//

#include "ofMain.h"

//in normalised screen coords -0.5>0.5


class ofxGrabCam : public ofCamera {
public:	
	ofxGrabCam();
	~ofxGrabCam();
	
	void	begin(ofRectangle viewport = ofGetCurrentViewport());
	void	end(); ///< Overrides virtual end
	void	reset();
	
	void drag(ofVec2f p_origin, ofVec2f p_now, ofVec3f mouseW);
	
	// convert between spaces
	ofVec3f getZPlaneProjection(ofVec2f screenPoint, ofRectangle viewport);
	ofVec3f unPickCoordinate(ofVec3f t_mouseW);
	ofVec3f	pickCoordinate(ofVec2f t_mouseP);
	
protected:
	

	void	update(ofEventArgs & args);
	//
	bool	initialised;

	////
	//view
	ofRectangle viewportRect;
	//
	GLint		viewport[4];
	GLdouble	matM[16], matP[16];
	//

};