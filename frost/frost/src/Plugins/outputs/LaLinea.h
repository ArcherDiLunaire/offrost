#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"
#include "ofxMSASpline.h"
#include "contourSimplify.h"
#include "contourNormals.h"

class LaLinea : public Output{
public:
	LaLinea();
	
	ofxMSASpline2D  spline;

	void setup();
	void draw();
	void update();
	void drawOnWall();
	
	void useFilm();
	void useCam();
	
	bool debug;
	int cam;
	
	contourSimplify contourSimp;
	contourNormals contourNorm;
	
	void drawContour(vector<ofxVec2f> * _points, float _lineWidth, float _extrusion);

	float extrude;
	float lineWidth;
	
	float useFilmUglyFloat;
	bool bUsingFilm;

	float yPosition;
	ofxVec2f camOffset(float x);
};