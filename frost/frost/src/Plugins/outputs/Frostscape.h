#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"
#include "ofxMSASpline.h"
#include "contourSimplify.h"
#include "contourNormals.h"
#include "FrostMotor.h"

class BlackSpotObject{
public:
	BlackSpotObject();
	vector<ofxVec2f> points;
	vector<ofxVec2f> pointsV;

	vector<vector<ofxVec2f> > noise;
	
	void updateBlob(ofxCvBlob b, PluginController * controller, BlackSpotObject * otherObject);
	void draw();
	contourSimplify contourSimp;
	contourNormals contourNorm;
	static const int numNoise = 3;
	
	
	ofxVec2f centroidV;
	vector<ofxVec2f> tmpPoints, tmpPoints2, normals;
	
	
};


class IceBlockJoint {
public:
	ofxVec2f position;
	bool springJoint;
	ofxVec2f force;
	ofxVec2f speed;
	vector<IceBlockJoint*> * joints;
	
	void update();
};


class IceBlock {
public:
	IceBlock();
	
	void draw();
	void update();
	
	void setBlobPoints(vector<ofxVec2f> points);
	bool pointInside(ofxVec2f  joint);
	
	float masterAlpha;
	
	vector<ofxVec2f> blobPoints;
	vector<IceBlockJoint*> joints;
};

class Frostscape : public Output{
public:
	Frostscape();
	
	void setup();
	void draw();
	void drawOnFloor();
	void update();
	
	vector<BlackSpotObject> blackSpots;
	vector<IceBlock> iceBlocks;

	FrostMotor motor;
	
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