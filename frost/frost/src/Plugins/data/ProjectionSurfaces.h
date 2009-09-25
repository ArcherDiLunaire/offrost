#pragma once
#include "ofMain.h"
#include "Data.h"
#include "ofxVectorMath.h"
#include "Warp.h"
#include "ofxXmlSettings.h"
#include "coordWarp.h"

class ProjectionSurfacesObject {
public:
	float aspect;
	Warp * warp;
	coordWarping * coordWarp;
	string name;
	
	ofxPoint2f * corners[4];
	
	ProjectionSurfacesObject();
	
	void recalculate();
	void SetCorner(int n, float x, float y);
};


class ProjectionSurfaces : public Data{
public:
	ProjectionSurfaces();
	
	void draw();
	void drawOnFloor();

	void setup();
	void update();
	
	vector<ProjectionSurfacesObject *> objects;

	
	void applyFloorProjection(float w=ofGetWidth(), float h=ofGetHeight());
	void applyColumnProjection(int column, float w=ofGetWidth(), float h=ofGetHeight());
	void applyCurtainProjection(int column, int row, float w=ofGetWidth(), float h=ofGetHeight());
	void applyWallProjection(float w=ofGetWidth(), float h=ofGetHeight());
	void applyProjection(ProjectionSurfacesObject * obj, float w=ofGetWidth(), float h=ofGetHeight());
	
	ProjectionSurfacesObject * getFloor();
	ProjectionSurfacesObject * getColumn(int n);
	ProjectionSurfacesObject * getCurtain(int n);
	ProjectionSurfacesObject * getWall();

	ofxPoint2f getColumnCoordinate(int column);
	
	ofxVec2f  convertToFloorCoordinate(ofxVec2f v);
		ofxVec2f  convertToWallCoordinate(ofxVec2f v);
	ofxVec2f  convertToCoordinate(ProjectionSurfacesObject * obj,  ofxVec2f v);

	
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
