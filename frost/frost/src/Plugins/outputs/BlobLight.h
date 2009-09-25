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
	float blur, threshold, blur2;
	float alpha, beta;
	int w,h;
	ofxCvGrayscaleImage history;
	ofxCvGrayscaleImage historyTmp;

	ofxCvGrayscaleImage img;

};