#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"
#include "ofxMSASpline.h"
#include "contourSimplify.h"
#include "contourNormals.h"
class BlackSpotObject{
public:
	BlackSpotObject();
	vector<ofxVec2f> points;
	vector<ofxVec2f> pointsV;
	vector<vector<ofxVec2f> > noise;
	
	void updateBlob(ofxCvBlob b, PluginController * controller);
	void draw();
	contourSimplify contourSimp;
	contourNormals contourNorm;
	static const int numNoise = 5;
	
	
	
	vector<ofxVec2f> tmpPoints, tmpPoints2, normals;
	
	
};

class Frostscape : public Output{
public:
	Frostscape();
	
	void setup();
	void draw();
	void drawOnFloor();
	void update();
	
	vector<BlackSpotObject> blackSpots;
	
	bool debug;
	int cam;
	
	static float randomFactor;
	static float slider1, slider2, slider3, slider4, slider5, slider6;
	
	void setslider1(float val);
	void setslider2(float val);
	void setslider3(float val);
	void setslider4(float val);
	void setslider5(float val);
	void setslider6(float val);
};