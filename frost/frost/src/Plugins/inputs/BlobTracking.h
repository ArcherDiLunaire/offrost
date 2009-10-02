#pragma once
#include "ofMain.h"
#include "Input.h"

#include "ofxOpenCv.h"
#include "ofxVideoGrabber.h"
#include "contourSimplify.h"
#include "ofxXmlSettings.h"
#include "ofxCvOpticalFlowLK.h"


class CameraCalibration;

class ofxExtendedBlobTracking : public ofxCvContourFinder{
public:
	int findSimplifiedContours( ofxCvGrayscaleImage&  input, int minArea, int maxArea, int nConsidered, bool bFindHoles, bool bUseApproximation) ;
};

class PersistentBlob {
public:
	PersistentBlob();
	static unsigned long int idCounter;

	long unsigned int id;
	ofxPoint2f centroid;
	ofxPoint2f lastcentroid;
	ofxVec2f centroidV;

	int timeoutCounter;
	vector<ofxCvBlob> blobs;
};

class Tracker {
public:
	Tracker();
	void setup();
	void update();
	void findContours();
	int cameraId;
	
	ofxPoint2f blackCorners[4]; 
	ofxXmlSettings * xml;

	
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayLastImage;
	ofxCvGrayscaleImage 	grayImageBlured;

	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	
	ofxCvContourFinder 	contourFinder;
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

	vector<ofxCvBlob> getBlobById(unsigned long int _id);
	int numPersistentBlobs();
	bool persistentBlobExist(unsigned long int _id);
	unsigned long int getPersistentBlobId(int n);
	void deletePersistentBlobById(unsigned long int id);

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
	
	vector<PersistentBlob> persistentBlobs;
	
	//ofxCvOpticalFlowLK	opticalFlow;

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
