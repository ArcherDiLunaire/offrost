/*
 *  MirrorBall.h
 *  openFrameworks
 *
 *  Created by frost on 06/10/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"

class MirrorBall: public Output{
	
public:
	
	MirrorBall();
	
	ofImage mirrorBallImage;
	
	void setup();
	void drawOnFloor();
	void draw();
	void update();
	void addDot(float x, float y);
	
	void drawMirrorBall(int numCircles);
	
	int cam;
	
	float rotation;
	float rotationDirection;
	
	vector<ofxVec2f> dots;
	
	ofxVec2f center;
	
};