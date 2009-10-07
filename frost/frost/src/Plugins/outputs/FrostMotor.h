#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"


class IceBlockBackgroundObject {
public:	

	ofxVec2f position;
	float a;
	vector<ofxVec2f> points;
	IceBlockBackgroundObject(float x, float y);
	void draw();
	void generate();
	vector<int> closeBackgrounds;
	int upTimer;
	int downTimer;
};


class FrostMotor{
public:
	FrostMotor();
	
	void setup();
	void draw();
	void drawOnFloor();
	void update();

	vector<IceBlockBackgroundObject> iceblockBackgrounds;

	bool debug;
	int cam;
	
	static float randomFactor;
	static float slider1, slider2, slider3, slider4, slider5, slider6;
	static bool applyToOther;
	
	void setslider1(float val);
	void setslider2(float val);
	void setslider3(float val);
	void setslider4(float val);
	void setslider5(float val);
	void setslider6(float val);
};