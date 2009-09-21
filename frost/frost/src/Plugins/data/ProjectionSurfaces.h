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
	void applyProjection(ProjectionSurfacesObject * obj, float w=ofGetWidth(), float h=ofGetHeight());
	
	ProjectionSurfacesObject * getFloor();
	ProjectionSurfacesObject * getColumn(int n);
	ProjectionSurfacesObject * getCurtain(int n);
	
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
