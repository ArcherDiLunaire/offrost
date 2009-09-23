#pragma once
#include "ofMain.h"
#include "Data.h"
#include "ofxVectorMath.h"
#include "Warp.h"
#include "ofxXmlSettings.h"
#include "coordWarp.h"

class CameraCalibrationObject {
public:
	Warp * warp;
	coordWarping * coordWarp;
	string name;
};


class CameraCalibration : public Data{
public:
	CameraCalibration();
	
	void draw();
	void drawOnFloor();

	void setup();
	void update();
	
	vector<CameraCalibrationObject *> cameras;
	
	bool drawDebug;
	ofTrueTypeFont	verdana;
	
	void drawSettings();
	
	ofxVec2f lastMousePos;
	int selectedCorner;
	int selectedKeystoner;
	
	void mousePressed(ofMouseEventArgs & args);
	void mouseDragged(ofMouseEventArgs & args);
	void keyPressed(ofKeyEventArgs & args);
	
	void saveXml();

	void guiWakeup();
	
	int w,h, offset;
	
	ofxXmlSettings * keystoneXml;
	
	void applyWarp(int cam, float _w=ofGetWidth(), float _h=ofGetHeight());
	

};
