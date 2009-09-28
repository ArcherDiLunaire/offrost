#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"
#include "ofxMSASpline.h"
#include "contourSimplify.h"
#include "contourNormals.h"

class Folding : public Output{
public:
	Folding();
	
	void setup();
	void draw();
	void update();
	
	
	bool debug;
	int cam;
	
};