#pragma once

#include "ofMain.h"
#include "Plugin.h"

//extern PluginController * pluginController;

class PluginController {
public:
	//PluginController();
	
	vector<FrostPlugin*> plugins;
	
	void addPlugin(FrostPlugin* plugin);
	void setup();
	void update();
	void draw();
	
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
