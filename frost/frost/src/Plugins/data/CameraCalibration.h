#pragma once
#include "ofMain.h"
#include "Data.h"
#include "ofxVectorMath.h"
#include "Warp.h"
#include "ofxXmlSettings.h"
#include "coordWarp.h"

class CameraCalibrationObject {
public:
	float aspect;
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
	
	vector<CameraCalibrationObject *> objects;
	
	void applyFloorProjection(float w=ofGetWidth(), float h=ofGetHeight());
	void applyColumnProjection(int column, float w=ofGetWidth(), float h=ofGetHeight());
	void applyCurtainProjection(int column, int row, float w=ofGetWidth(), float h=ofGetHeight());
	void applyProjection(CameraCalibrationObject * obj, float w=ofGetWidth(), float h=ofGetHeight());
	
	CameraCalibrationObject * getFloor();
	CameraCalibrationObject * getColumn(int n);
	CameraCalibrationObject * getCurtain(int n);
	
	ofxPoint2f getColumnCoordinate(int column);
	
	bool drawDebug;
	ofTrueTypeFont	verdana;
	
	void drawSettings();
	void drawDebugGrids(int _w=ofGetWidth(), int _h= ofGetHeight());
	
	
	ofxVec2f lastMousePos;
	int selectedCorner;
	
	void mousePressed(ofMouseEventArgs & args);
	void mouseDragged(ofMouseEventArgs & args);
	void keyPressed(ofKeyEventArgs & args);
	
	void saveXml();

	void guiWakeup();
	
	void drawGrid(string text, float aspect, int resolution,  bool drawBorder, float alpha=1.0, float fontSize=1.0);
	
	int w,h, offset;
	
	ofxXmlSettings * keystoneXml;
	
	int selectedKeystoner;
};
