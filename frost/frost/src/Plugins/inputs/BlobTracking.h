#pragma once
#include "ofMain.h"
#include "Input.h"

#include "ofxOpenCv.h"
#include "ofxVideoGrabber.h"
#include "contourSimplify.h"




class CameraCalibration;

class ofxExtendedBlobTracking : public ofxCvContourFinder{
public:
	int findSimplifiedContours( ofxCvGrayscaleImage&  input, int minArea, int maxArea, int nConsidered, bool bFindHoles, bool bUseApproximation) ;
};

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
	
	ofxExtendedBlobTracking 	contourFinder;
//	ofxExtendedBlobTracking 	simplifiedContourFinder;
	PluginController * controller;
	
	float 				threshold;
	float				blur;
	bool active;
	
	bool				bLearnBakground;
	
	bool mouseBlob;
	ofxCvBlob mouseGeneratedBlob;
	void updateMouseBlob(float x, float y, int button);
	
	
	//Getters
	int numBlobs();
	ofxCvBlob getConvertedBlob(ofxCvBlob * blob, CameraCalibration * calibrator);
	ofxCvBlob getBlob(int n);
	ofxCvBlob getLargestBlob();
	int getWidth();
	int getHeight();
/*
	void smoothBlobs(float smooth);
	void extrudeBlobs(float value);	
*/
	ofxCvBlob smoothBlob(ofxCvBlob blob, float smooth);
	void extrudeBlob(ofxCvBlob * blob, float value);	
	
	contourSimplify contourSimp;
	
	float postBlur, postThreshold;

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
