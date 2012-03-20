#pragma once

#include "utils.h"
#include "reader.h"

class ofxGrabCam : public ofCamera {
	
public:	
	ofxGrabCam();
	~ofxGrabCam();
	
	void	begin(ofRectangle viewport = ofGetCurrentViewport());
	void	end(); // Overrides virtual end
	void	reset();
	
	void calcFollowPoint(reader * currentReader,  ofRectangle t_viewPort);
	void followReader(reader * currentReader, ofRectangle t_viewPort);
	void pauseFollow();
	void restartFollow();
	void toggleFollow();
	void calcTrans();
	
	
	void incrementRotations();
	void drag(ofVec2f p_origin, ofVec2f p_now, ofVec3f mouseW);
	void zoom(float t_amount);
	
	// convert between spaces
	ofVec3f getZPlaneProjection(ofVec2f screenPoint, ofRectangle viewport);
	ofVec3f unPickCoordinate(ofVec3f t_mouseW);
	ofVec3f	pickCoordinate(ofVec2f t_mouseP);
	
	void setTargetRotation(float rot, int t_axis);
	
	//getters and setters
	float getRotation(int t_axis);
	bool getIsFixed();
	ofVec2f getTrans();
	void setWorldDims(ofVec2f t_dims);
	
	
protected:
	
	bool	initialised;
	
	//testApp stuff
	const float kLagFrames;
	const float kMinIncr;
	int lagCount;
	bool isFixed, isPaused;
	ofVec2f trans, direction, followPoint, worldDims;
	int cReaderIndex;

	//rotations
	float rots[3];
	float targetRots[3];
	
	//matrices etc
	ofRectangle viewportRect;
	GLint		viewport[4];
	GLdouble	matM[16], matP[16];


};