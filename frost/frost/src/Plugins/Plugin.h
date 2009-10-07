#pragma once

#include "ofMain.h"
#include "CustomGLViewDelegate.h"


class PluginController;
class ProjectionSurfaces;
class Tracker;

class FrostPlugin {
public:
	enum Type {
		INPUT,
		DATA,
		OUTPUT
	};
	
	int type;
	bool enabled;
	
	virtual void guiWakeup(){};
	CustomGLViewDelegate * glDelegate;
	float dt;
	
	PluginController * controller;
	
	FrostPlugin();
	virtual ~FrostPlugin(){};

	virtual void setup(){};
	virtual void draw(){};
	virtual void update(){};
	
	virtual void drawOnFloor(){};
	virtual void drawOnWall(){};
	
	void applyFloorProjection();
	void applyWallProjection();

	ProjectionSurfaces* projection();
	Tracker* blob(int n);

	float mouseX, mouseY;
};