#pragma once

#include "ofMain.h"
#include "Plugin.h"

//extern PluginController * pluginController;

class PluginController {
public:
	//Plugincontroller;
	
	vector<FrostPlugin*> plugins;
	
	float projectorMaskAlpha;
	
	ofImage projectorMask;

	void addPlugin(FrostPlugin* plugin);
	void setup();
	void update(float mx, float my);
	void draw();
	void drawFloor();
	void drawWall();
	void drawMasking();
};





template <class T>
T getPlugin (PluginController * pluginController) {
	for(int i=0;i<pluginController->plugins.size();i++){
		if(dynamic_cast<T> (pluginController->plugins[i]) != NULL){
			return 	(T)pluginController->plugins[i];
		}
	}		
	return NULL;
};
