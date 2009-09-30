#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"
#include "ofxMSAFluid.h"

class LiquidSpace : public Output{
public:
	LiquidSpace();
	
	void setup();
	void draw();
	void update();
	
	
	bool debug;
	int cam;
	
};