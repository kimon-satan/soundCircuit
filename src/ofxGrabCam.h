#pragma once

#include "constants.h"
#include "reader.h"

class ofxGrabCam : public ofCamera {
	
public:	
	ofxGrabCam();
	~ofxGrabCam();
	
	void	begin(ofRectangle viewport = ofGetCurrentViewport());
	void	end(); // Overrides virtual end
	void	reset();
	
	void followReader(reader * currentReader, ofVec2f world_dims);
	void pauseFollow();
	void restartFollow();
	void toggleFollow();
	
	void incrementRotations();
	void drag(ofVec2f p_origin, ofVec2f p_now, ofVec3f mouseW);
	
	// convert between spaces
	ofVec3f getZPlaneProjection(ofVec2f screenPoint, ofRectangle viewport);
	ofVec3f unPickCoordinate(ofVec3f t_mouseW);
	ofVec3f	pickCoordinate(ofVec2f t_mouseP);
	
	void setTargetRotation(float rot, int t_axis);
	float getRotation(int t_axis);
	
	
protected:
	
	bool	initialised;
	
	//testApp stuff
	const float kLagFrames;
	const float kMinIncr;
	int lagCount;
	bool isFixed, isPaused;
	ofVec2f trans, direction, followPoint;

	//rotations
	float rots[3];
	float targetRots[3];
	
	//matrices etc
	ofRectangle viewportRect;
	GLint		viewport[4];
	GLdouble	matM[16], matP[16];


};