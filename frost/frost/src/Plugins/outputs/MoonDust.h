#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"

class DustParticle : public ofxVec3f {
public:
	DustParticle(float x, float y, float z);
};

class MoonDust : public Output{
public:
	MoonDust();
	
	void draw();
	void update();
	
	vector<DustParticle> particles;
	
	ofImage particleImg;

};