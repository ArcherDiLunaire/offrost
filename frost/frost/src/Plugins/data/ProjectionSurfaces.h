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
	
	void applyFloorProjection();
	
	bool drawDebug;

	
};
