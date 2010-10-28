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
#include "Filter.h"

class lamp{
	
public:
	
	lamp();
	int r, g, b, a;
	ofxPoint2f pos;
	int channel;
	
	int sentR, sentG, sentB, sentA;
	
	bool isOldAndSucks;
	
	
	

	void update();
	
};

class LEDGrid;

class LEDGridThread : public ofxThread {
	
public:
	
		bool connected;

	LEDGridThread();
	
	void threadedFunction();
	void start();
	void stop();
	
	float master;
	float sentMaster;
	
	bool ok;
	
	ofSerial serial;
	vector<lamp> lamps;
	
	float radius;
	
	float r,g,b;
	float r2,g2,b2;
	
	bool alphaSet;
	
	ofxPoint2f p;
	
	LEDGrid * ledGridObject;
	
	bool debugFlag;
	
	int byteCounter;
	
	
};

class LEDGrid: public Output{
	
public:
	
	LEDGrid();
	
	void setup();
	void drawOnFloor();
	void draw();
	void update();
	void updateThreadValues();
	
	bool debug;

	int cam;
	
	float master;
	
	bool ok;

	ofSerial serial;
	vector<lamp> lamps;
	
	float radius;
	
	float r,g,b;
	float r2,g2,b2;
	
	bool alphaSet;
	
	ofxPoint2f p;
	
	LEDGridThread serialThread;
	
	float pt1x, pt1y, pt2x, pt2y;
	Filter * filterx;
		Filter * filtery;
	
};