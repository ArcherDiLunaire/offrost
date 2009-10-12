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

class MirrorBallReflections {
	
public:
	
	MirrorBallReflections();
	
	void setup(int _numberOfCircles, float _circlesSpacing, float _circlesSpacingMultiplier, float _circleOffsetX, float _circleOffsetY,
			   float _dotSize, float _dotSpacing, float _dotSizeDistanceMultiplier,
			   float _dotPositionNoise, float _dotAlphaNoise);
	
	void draw(float _angle);
	void update();
	int getNumberOfDots(int _numberOfCircles, float _dotSpacing);
	
	void setDotStates(int amountOfDots, bool isOn);
	void turnOnSomeDots(int amountOfDots);
	void turnOffSomeDots(int amountOfDots);
	void clearReflections();
	void allReflections();
	
	float numberOfCircles;
	float circlesSpacing;
	float circlesSpacingMultiplier;
	float circleOffsetX;
	float circleOffsetY;
	
	float dotSize;
	float dotSpacing;
	float dotSizeDistanceMultiplier;
	float dotPositionNoise;
	float dotAlphaNoise;
	
	float fractionOfDotsTurnedOn;
	float fractionOfDotsShouldBeTurnedOn;

	float alpha;

	int nDots;
	int nDotsTurnedOn;
	
	vector<bool> dotStates;
	

	
	ofxVec2f center;
	
};

class MirrorBall: public Output{
	
public:

	MirrorBall();

	ofImage mirrorBallImage;
	
	void setup();
	void drawOnFloor();
	void draw();
	void update();
	void addDot(float x, float y);
	
	int cam;
	
	MirrorBallReflections reflections1;
	MirrorBallReflections reflections2;
	
	float rotation;
	float rotationDirection;
	
	float mirrorBallTrails;
	
	bool bAddDot;
	bool bClearDots;
};