#pragma once
#include "ofMain.h"
#include "Data.h"
#include "ofxVectorMath.h"
#include "Warp.h"

class ProjectionSurfaces : public Data{
public:
	ProjectionSurfaces();
	
	void draw();
	void setup();
	void update();
	
	Warp * floorProjection;
	
	void applyFloorProjection(float w=ofGetWidth(), float h=ofGetHeight());
	
	bool drawDebug;

	int gridFloorResolution;
	
	void drawSettings();
	
	ofxVec2f lastMousePos;
	int selectedCorner;
	
	void mousePressed(ofMouseEventArgs & args);
	void mouseDragged(ofMouseEventArgs & args);
	void guiWakeup();
	
	int w,h, offset;
};
