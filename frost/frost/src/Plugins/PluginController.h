#pragma once

#include "ofMain.h"
#include "Plugin.h"

class PluginController {
public:
	//PluginController();
	
	vector<FrostPlugin*> plugins;
	
	void addPlugin(FrostPlugin* plugin);
	void update();
	void draw();
	
};




template <class T>
T getPlugin (PluginController * controller) {
	for(int i=0;i<controller->plugins.size();i++){
		if(dynamic_cast<T> (controller->plugins[i]) != NULL){
			return 	(T)controller->plugins[i];
		}
	}		
	return NULL;
};
