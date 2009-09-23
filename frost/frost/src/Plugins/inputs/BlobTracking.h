#pragma once
#include "ofMain.h"
#include "Input.h"

#include "ofxOpenCv.h"
#include "ofxVideoGrabber.h"

class Tracker {
public:
	Tracker(ofxVideoGrabber * grabber);
	void update();
	void findContours();
	ofxVideoGrabber * grabber;
	
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayImageBlured;

	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	
	ofxCvContourFinder 	contourFinder;
	
	float 				threshold;
	float				blur;
	bool active;
	
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
	
	void drawSettings();
	
	void setThreshold(int n, float v);
	void setBlur(int n, int v);
	void grab(int n);
	void setActive(int n, bool b);
	
	float initThreshold[3];
	float initBlur[3];
	bool initActive[3];
	
	bool drawDebug;


};
