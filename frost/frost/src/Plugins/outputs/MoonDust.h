#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"

class DustParticle : public ofxVec3f {
public:
	DustParticle(float x, float y, float z);
	void update(float force, float damp, float min, float max);
	float getAcc();
	ofxVec3f v;
	vector<ofxVec3f> history;
	ofxVec3f pos();
	float vEffect;
	bool goingMax;
};

class MoonDust : public Output{
public:
	MoonDust();

	void setup();
	void draw();
	void update();
	
	float force;
	float damp;
	vector<DustParticle> particles;
	
	float min, max;
	
	ofImage particleImg;
	ofImage particleTrack;
};