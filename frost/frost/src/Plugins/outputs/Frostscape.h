#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"
#include "ofxMSASpline.h"
#include "contourSimplify.h"
#include "contourNormals.h"
#include "FrostMotor.h"


class Frostscape : public Output{
public:
	Frostscape();
	
	void setup();
	void draw();
	void drawOnFloor();
	void update();
	
	FrostMotor motor;
	
	bool debug;
	int cam;
	
	static float randomFactor;
	static float slider1, slider2, slider3, slider4, slider5, slider6;
	static bool applyToOther;
	
	void setslider1(float val);
	void setslider2(float val);
	void setslider3(float val);
	void setslider4(float val);
	void setslider5(float val);
	void setslider6(float val);
};