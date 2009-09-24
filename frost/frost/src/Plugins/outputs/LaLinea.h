#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"


class LaLinea : public Output{
public:
	LaLinea();

	void setup();
	void draw();
	void update();
	void drawOnFloor();
	
	bool debug;
	int cam;
};