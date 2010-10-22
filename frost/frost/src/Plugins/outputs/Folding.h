#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"
#include "ofxMSASpline.h"
#include "contourSimplify.h"
#include "contourNormals.h"
//#include "ofFBOTexture.h"

class Folding : public Output{
public:
	Folding();
	
	void setup();
	void draw();
	void drawOnFloor();
	void update();
	
	void getTrackingRect(ofxPoint2f * corners);
	void drawImage(ofxCvGrayscaleImage img, float pushFactor);
	
	float aspect;
	float scale;
	float offsetX, offsetY;
	
	bool debug;
	bool updateHistoryFromBlob;
	bool reset;
	int cam;
	
	float historyAlpha;
	float historyAddMultiplier;

	ofxCvGrayscaleImage historyImg;
	ofxCvGrayscaleImage historyImgTemp;
	
	vector<GLuint> blobHistoryMatrixDisplayList;
	
//	ofFBOTexture fbo;
	ofTexture tex;
	
	vector<ofxCvGrayscaleImage> history;
	vector<ofxCvGrayscaleImage> now;
	
	int histPos;
	
	float push1, push2, push3;
	float fishAlpha;
	float foldingFloorbox;
	float foldingFloorboxAlpha;
	
	ofVideoPlayer fish;
};