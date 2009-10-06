#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"


class BlobLight : public Output{
public:
	BlobLight();

	void setup();
	void draw();
	void update();
	void drawOnFloor();
	
	bool debug;
	
	float r, g, b;
	float r2, g2, b2;
	float blur, threshold, blur2;
	float alpha, beta, blobalpha, historyalpha, addblack;
	int w,h;
	ofxCvGrayscaleImage history;
	ofxCvGrayscaleImage historyTmp;
	
	ofxCvGrayscaleImage img;
	
	ofxCvGrayscaleImage black;

};