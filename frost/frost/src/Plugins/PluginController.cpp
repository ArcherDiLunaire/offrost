

#include "PluginController.h"

void PluginController::addPlugin(FrostPlugin* plugin){
	plugin->controller = this;
	plugins.push_back(plugin);
}


void PluginController::setup(){
	for(int i=0;i<plugins.size();i++){
		FrostPlugin* plugin = plugins[i];
		plugin->setup(); 
	}
}

void PluginController::update(){
	for(int i=0;i<plugins.size();i++){
		FrostPlugin* plugin = plugins[i];
		if(plugin->enabled){
			plugin->update(); 
		}
	}
}
void PluginController::draw(){
	
	for(int i=0;i<plugins.size();i++){
		FrostPlugin* plugin = plugins[i];
		if(plugin->type == FrostPlugin::OUTPUT && plugin->enabled){
			plugin->draw(); 
		}
	}
	for(int i=0;i<plugins.size();i++){
		FrostPlugin* plugin = plugins[i];
		if(plugin->type == FrostPlugin::DATA && plugin->enabled){
			plugin->draw(); 
		}
	}
}

