#pragma once
#include "ofMain.h"
#include "Input.h"

#include "ofxOpenCv.h"
#include "ofxVideoGrabber.h"

class Tracker {
public:
	Tracker(ofxVideoGrabber * grabber);
	void update();
	
	ofxVideoGrabber * grabber;
	
	ofxCvColorImage		colorImg;
	
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	
	ofxCvContourFinder 	contourFinder;
	
	int 				threshold;
	bool				bLearnBakground;
};

class BlobTracking : public Input{
public:
	BlobTracking();
	
	void draw();
	void setup();
	void update();
	
	vector<Tracker*> trackers;
	
	float threshold;
	
};
