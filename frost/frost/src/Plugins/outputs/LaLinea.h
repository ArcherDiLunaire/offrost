#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"
#include "ofxMSASpline.h"

class LaLinea : public Output{
public:
	LaLinea();
	
	ofxMSASpline2D  spline;

	void setup();
	void draw();
	void update();
	void drawOnWall();
	
	bool debug;
	int cam;
};