#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"
#include "ofxMSASpline.h"
#include "contourSimplify.h"
#include "contourNormals.h"

class LaLineaFloor : public Output{
public:
	LaLineaFloor();
	
//	ofxMSASpline2D  spline;

	void setup();
	void draw();
	void update();
	void drawOnFloor();
	
	
	bool debug;
	int cam;

	vector <ofxPoint2f> pnts;
	ofxVec2f dir;
		ofxVec2f pos;
	float time;
	float lastTime;
	
	float width;
	float speed;
	float dirSpeed;
	float curlValue;
	void reset();
};