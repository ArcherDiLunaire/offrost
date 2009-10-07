#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"


class IceBlockBackgroundObject {
public:	

	ofxVec2f position;
	float a;
	vector<ofxVec2f> points;
	IceBlockBackgroundObject(float x, float y, float initA = 0.0, float size=1.0);
	void draw();
	void generate();
	vector<int> closeBackgrounds;
	int upTimer;
	int downTimer;
	float size;
	float speed;
};

class FreezePoint {
public:
	ofxPoint2f position;
	ofxVec2f dir;
	float rate;
};


class FrostMotor{
public:
	FrostMotor();
	
	void draw();
	void update();
	
	void setAreaValue(ofxPoint2f point, float radius, float value);
	void generateBackgroundObjects(int resolution, float objectSizes, float w, float h, float initValue=-3.0);

	void addBodyPoint(ofPoint p);
	void addBodyPoints(vector<ofPoint>* v);
	void addBodyCenter(ofPoint p);
	void addFreezePoint(ofPoint p, float rate);

	vector<IceBlockBackgroundObject> iceblockBackgrounds;

	vector<ofPoint> bodyPoints;
	vector<ofPoint> bodyCenters;
	vector<FreezePoint> freezePoints;

	
	float r;
	float rSq;
	
	float centerBreakRate;
	float bodyBreakRate;
	
	float expandRate;
	float decreaseRate;
	
};