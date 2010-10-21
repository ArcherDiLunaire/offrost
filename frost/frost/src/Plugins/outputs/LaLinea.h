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
	
	void drawContour(vector<ofxVec2f> * _points, float _lineWidth, float _extrusion, bool _reverse);

	float extrude;
	float lineWidth;
	
	bool bUsingFilm;
	bool bHasToUseFilm;
	
	float yPosition;
	ofxVec2f camOffset(ofxVec2f pointIn);
	
	ofxVec2f offsetPoint, offsetPoint2, offsetPointMovie, offsetPoint2Movie;
	
	bool tracking;
	
	float maskAlpha;
	
	float noise1, noise2, noise3;
	float dancer;
	
};