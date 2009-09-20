#pragma once
#include "ofMain.h"
#include "Data.h"
#include "ofxVectorMath.h"
#include "Warp.h"
#include "ofxXmlSettings.h"

class ProjectionSurfacesObject {
public:
	float * aspect;
	Warp * warp;
	string name;
};


class ProjectionSurfaces : public Data{
public:
	ProjectionSurfaces();
	
	void draw();
	void drawOnFloor();

	void setup();
	void update();
	
	Warp * floorProjection;
	float floorAspect;

	Warp * columnProjection[3];
	float columnAspect;

	Warp * curtainProjection[6];
	float curtainAspect[2];
	
	vector<ProjectionSurfacesObject *> objects;
	
	void applyFloorProjection(float w=ofGetWidth(), float h=ofGetHeight());
	void applyColumnProjection(int column, float w=ofGetWidth(), float h=ofGetHeight());
	void applyCurtainProjection(int column, int row, float w=ofGetWidth(), float h=ofGetHeight());
	void applyProjection(ProjectionSurfacesObject * obj, float w=ofGetWidth(), float h=ofGetHeight());

	bool drawDebug;
	
	ofTrueTypeFont	verdana;
	
	int gridFloorResolution;
	
	void drawSettings();
	void drawDebugGrids(int _w=ofGetWidth(), int _h= ofGetHeight());
	
	Warp * getWarp(int i);
	
	ofxVec2f lastMousePos;
	int selectedCorner;
	
	void mousePressed(ofMouseEventArgs & args);
	void mouseDragged(ofMouseEventArgs & args);
	void keyPressed(ofKeyEventArgs & args);
	
	void saveXml();

	void guiWakeup();
	
	void drawGrid(string text, float aspect, int resolution);
	
	int w,h, offset;
	
	ofxXmlSettings * keystoneXml;
	
	int selectedKeystoner;
};
