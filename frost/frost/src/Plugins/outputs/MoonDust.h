#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"

#define cam 0

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
	float min, max;
	bool visible;
	float alpha;
};

class MoonDust : public Output{
public:
	MoonDust();

	void setup();
	void draw();
	void update();
	void drawOnFloor();

	float force;
	float size, length, density;
	bool debug;
	vector<DustParticle> particles;
	
	float min, max;
	
	ofImage * particleImg;
	ofImage * particleTrack;
	
	float rotation;
};