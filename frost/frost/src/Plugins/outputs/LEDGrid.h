/*
 *  LEDGrid.h
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

class lamp{
	
public:
	
	lamp();
	int r, g, b, a;
	ofxPoint2f pos;
	int channel;
	
	int sentR, sentG, sentB, sentA;
	
	void update();
	
};

class LEDGrid: public Output{
	
public:
	
	LEDGrid();
	
	void setup();
	void drawOnFloor();
	void draw();
	void update();
	
	int cam;
	
	vector<lamp> lamps;

	bool ok;
	ofSerial serial;
	
	bool debug;
	
	float radius;
	
	float r,g,b;
	float r2,g2,b2;
	
	ofxPoint2f p;
	
};