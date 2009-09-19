#pragma once

#include "ofMain.h"
#include "CustomGLViewDelegate.h"


class PluginController;

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
		virtual void setup(){};
	virtual void draw(){};
	virtual void update(){};
	
	void applyFloorProjection();
};

