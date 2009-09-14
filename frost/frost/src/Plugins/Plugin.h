#pragma once

#include "ofMain.h"


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
	
	PluginController * controller;
	
	
	FrostPlugin();
	virtual void draw(){};
	virtual void update(){};
};

