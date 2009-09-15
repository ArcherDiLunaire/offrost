#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"

class DustParticle : public ofxVec3f {
public:
	DustParticle(float x, float y, float z);
	void update(float force, float damp);
	ofxVec3f v;
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
	
	ofImage particleImg;

};