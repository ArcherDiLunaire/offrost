#pragma once
#include "ofMain.h"
#include "Input.h"

#include "ofxOpenCv.h"
#include "ofxVideoGrabber.h"

class CameraCalibration;

class Tracker {
public:
	Tracker();
	void update();
	void findContours();
	int cameraId;
	
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayImageBlured;

	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	
	ofxCvContourFinder 	contourFinder;
	PluginController * controller;
	
	float 				threshold;
	float				blur;
	bool active;
	
	bool				bLearnBakground;
	
	//Getters
	int numBlobs();
	ofxCvBlob getConvertedBlob(ofxCvBlob * blob, CameraCalibration * calibrator);
	ofxCvBlob getBlob(int n);
	ofxCvBlob getLargestBlob();
	int getWidth();
	int getHeight();
	
private:
	int cw, ch;
	
	
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
	
	//Getters
	//Tracker* tracker(int n);
	


};
